#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4305 )
#pragma warning ( disable : 4786 )
#endif

#include "qwt3d_surfaceplot.h"
#include "qwt3d_enrichment_std.h"

using namespace std;
using namespace Qwt3D;


/////////////////////////////////////////////////////////////////////////////////
//
//     cell specific
//


void SurfacePlot::createDataC()
{		
	createFloorDataC();
  



  if (plotStyle() == NOPLOT)
    return;

 /* if (plotStyle() == Qwt3D::POINTS)
  {
    createPoints();
    return;
  }*/
  else if (plotStyle() == Qwt3D::USER)
  {
    if (userplotstyle_p)
      createEnrichment(*userplotstyle_p);
    return;
  }

	setDeviceLineWidth(meshLineWidth());
  GLStateBewarer sb(GL_POLYGON_OFFSET_FILL,true);
	setDevicePolygonOffset(polygonOffset(),1.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	int idx = 0;
	if (plotStyle() != WIREFRAME)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);
    glLineWidth(3);
   



		bool hl = (plotStyle() == HIDDENLINE);
		if (hl)
		{
		
		}
	  RGBA col = backgroundRGBAColor();
	 // glColor4d(col.r, col.g, col.b, col.a);
    //for (unsigned gr = 0; gr !=actualDataC_->groups.size(); ++gr)
    //{

    for (unsigned gr=0; gr!=actualDataC_->groups.size(); ++gr)
    {
      if (actualDataC_->groups[gr].enabled)
		  for (unsigned i=0; i!=actualDataC_->groups[gr].lines.size(); ++i)
		  {
      if ( actualDataC_->groups[gr].lines[i].type != 1   )
      {
        Label LineCaption;
        LineCaption.setString( actualDataC_->groups[gr].lines[i].name);
        LineCaption.setPosition(actualDataC_->nodes[idx]);
        LineCaption.setColor(actualDataC_->groups[gr].lines[i].r, actualDataC_->groups[gr].lines[i].g, actualDataC_->groups[gr].lines[i].b, col.a);
        LineCaption.setFont("Courier", 10, QFont::Normal, false);
        LineCaption.adjust(0);
        LineCaption.draw();
      }

      if ((actualDataC_->groups[gr].lines[i].type != 0   ) || (plotStyle() == Qwt3D::POINTS))
      {
        if ( plotStyle() == Qwt3D::POINTS ) 
          glPointSize(5);
        if ( actualDataC_->groups[gr].lines[i].type == 1   )
          glPointSize(10);

        glEnable(GL_POINT_SMOOTH);
        glBegin(GL_POINTS);
		
      }
      else
      {
        glEnable(GL_LINE_SMOOTH);
        glBegin(GL_LINE_STRIP);
      }

      for (unsigned j=0; j!=actualDataC_->groups[gr].lines[i].Cell.size(); ++j)
			{
 

               idx = actualDataC_->groups[gr].lines[i].Cell[j];
			//	setColorFromVertexC(idx, hl);
        glColor4d(actualDataC_->groups[gr].lines[i].r, actualDataC_->groups[gr].lines[i].g, actualDataC_->groups[gr].lines[i].b, col.a);
				glVertex3d( actualDataC_->nodes[idx].x, actualDataC_->nodes[idx].y, actualDataC_->nodes[idx].z );
			//	glNormal3d( actualDataC_->normals[idx].x, actualDataC_->normals[idx].y, actualDataC_->normals[idx].z );
			}
			glEnd();
		}
    }
	}

	/*if (plotStyle() == FILLEDMESH || plotStyle() == WIREFRAME || plotStyle() == HIDDENLINE)
	{
		glColor4d(meshColor().r, meshColor().g, meshColor().b, meshColor().a);
		{
			for (unsigned i=0; i!=actualDataC_->cells.size(); ++i)
			{
				glBegin(GL_LINE_LOOP);
        for (unsigned j=0; j!=actualDataC_->cells[i].Cell.size(); ++j)
				{
          idx = actualDataC_->cells[i].Cell[j];
					glVertex3d( actualDataC_->nodes[idx].x, actualDataC_->nodes[idx].y, actualDataC_->nodes[idx].z );
				}
				glEnd();
			}
		}
	}*/
}

// ci = cell index
// cv = vertex index in cell ci
void SurfacePlot::setColorFromVertexC(int node, bool skip)
{
	if (skip)
		return;

	RGBA col = (*datacolor_p)(
		actualDataC_->nodes[node].x, actualDataC_->nodes[node].y, actualDataC_->nodes[node].z);
		
	glColor4d(col.r, col.g, col.b, col.a);
}

void SurfacePlot::createFloorDataC()
{
	switch (floorStyle())
	{
	case FLOORDATA:
		Data2FloorC();
		break;
	case FLOORISO:
		Isolines2FloorC();
		break;
	default:
		break;
	}
}

void SurfacePlot::Data2FloorC()
{	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	double zshift = actualDataC_->hull().minVertex.z;
	int idx;

  for (unsigned gr=0; gr!=actualDataC_->groups.size(); ++gr)
	for (unsigned i = 0; i!=actualDataC_->groups[gr].lines.size(); ++i)
	{
		glBegin(GL_POLYGON);
    for (unsigned j=0; j!=actualDataC_->groups[gr].lines[i].Cell.size(); ++j)
		{
      idx = actualDataC_->groups[gr].lines[i].Cell[j];
			setColorFromVertexC(idx);
			glVertex3d( actualDataC_->nodes[idx].x, actualDataC_->nodes[idx].y, zshift );
		}
		glEnd();
	}
}

void SurfacePlot::Isolines2FloorC()
{
	if (isolines() <= 0 || actualData_p->empty())
		return;

	double step = (actualData_p->hull().maxVertex.z - actualData_p->hull().minVertex.z) / isolines();		

	RGBA col;

	double zshift = actualData_p->hull().minVertex.z;
		
	TripleField nodes;
	TripleField intersection;
	
	double lambda = 0;
	
	GLStateBewarer sb2(GL_LINE_SMOOTH, false);

	for (int k = 0; k != isolines(); ++k) 
	{
		double val = zshift + k * step;		
		for (unsigned gr=0; gr!=actualDataC_->groups.size(); ++gr)		
		for (unsigned i=0; i!= actualDataC_->groups[gr].lines.size(); ++i)
		{
			nodes.clear();
      unsigned cellnodes = actualDataC_->groups[gr].lines[i].Cell.size();
			for (unsigned j=0; j!=cellnodes; ++j)
			{
        nodes.push_back(actualDataC_->nodes[actualDataC_->groups[gr].lines[i].Cell[j]]);
			}
			
			double diff = 0;
			for (unsigned m = 0; m!=cellnodes; ++m)
			{
				unsigned mm = (m+1)%cellnodes;
				if ((val>=nodes[m].z && val<=nodes[mm].z) || (val>=nodes[mm].z && val<=nodes[m].z))
				{
					diff = nodes[mm].z - nodes[m].z;
					
					if (isPracticallyZero(diff)) // degenerated
					{
						intersection.push_back(nodes[m]);
						intersection.push_back(nodes[mm]);
						continue;
					}
					
					lambda =  (val - nodes[m].z) / diff;
					intersection.push_back(Triple(nodes[m].x + lambda * (nodes[mm].x-nodes[m].x), nodes[m].y + lambda * (nodes[mm].y-nodes[m].y), val));
				}
			}

			if (!intersection.empty())
			{
				col = (*datacolor_p)(nodes[0].x,nodes[0].y,nodes[0].z);
  			glColor4d(col.r, col.g, col.b, col.a);
				if (intersection.size()>2)
				{
					glBegin(GL_LINE_STRIP);
					for (unsigned dd = 0; dd!=intersection.size(); ++dd)
					{
						glVertex3d(intersection[dd].x, intersection[dd].y, zshift);
					}
					glEnd();
					glBegin(GL_POINTS);
            glPointSize(10);
						glVertex3d(intersection[0].x,intersection[0].y,zshift);
					glEnd();
				}
				else if (intersection.size() == 2)
				{
					glBegin(GL_LINES);
						glVertex3d(intersection[0].x,intersection[0].y,zshift);
						glVertex3d(intersection[1].x,intersection[1].y,zshift);
						
						// small pixel gap problem (see OpenGL spec.)
						glVertex3d(intersection[1].x,intersection[1].y,zshift);
						glVertex3d(intersection[0].x,intersection[0].y,zshift);
					glEnd();
				}
				
				intersection.clear();
			}
		}
	}
}

void SurfacePlot::createNormalsC()
{
	if (!normals() || actualData_p->empty())
		return;

	if (actualDataC_->nodes.size() != actualDataC_->normals.size())
		return;
  Arrow arrow;
  arrow.setQuality(normalQuality());

	Triple basev, topv, norm;	
		
	double diag = (actualData_p->hull().maxVertex-actualData_p->hull().minVertex).length() * normalLength();

  RGBA col;
  arrow.assign(*this);
  arrow.drawBegin();
	for (unsigned i = 0; i != actualDataC_->normals.size(); ++i) 
	{
		basev = actualDataC_->nodes[i];
		topv = basev + actualDataC_->normals[i];
		
			norm = topv-basev;
			norm.normalize();
			norm	*= diag;

      arrow.setTop(basev+norm);
      arrow.setColor((*datacolor_p)(basev.x,basev.y,basev.z));
      arrow.draw(basev);
	}
  arrow.drawEnd();
}

/*! 
	Convert user (non-rectangular) mesh based data to internal structure.
	See also Qwt3D::TripleField and Qwt3D::CellField
*/
void SurfacePlot::ResetData()
{
  actualDataG_->clear();

}
int SurfacePlot::nodesCount()
{
  return actualDataC_->nodes.size();
}

void SurfacePlot::setGroupVisible(const QString& groupName, bool shown)
{
  for (unsigned int it = 0 ; it < actualDataC_->groups.size() ; ++it)
    if ( actualDataC_->groups[it].name == groupName)
      actualDataC_->groups[it].enabled = shown;
  //this->
}

bool SurfacePlot::UpdateData()
{
  unsigned i;

	ParallelEpiped hull(Triple(DBL_MAX,DBL_MAX,DBL_MAX),Triple(-DBL_MAX,-DBL_MAX,-DBL_MAX));

	for (i = 0; i!= actualDataC_->nodes.size(); ++i)
	{
		if (actualDataC_->nodes[i].x < hull.minVertex.x)
			hull.minVertex.x = actualDataC_->nodes[i].x;
		if (actualDataC_->nodes[i].y < hull.minVertex.y)
			hull.minVertex.y = actualDataC_->nodes[i].y;
		if (actualDataC_->nodes[i].z < hull.minVertex.z)
			hull.minVertex.z = actualDataC_->nodes[i].z;
		
		if (actualDataC_->nodes[i].x > hull.maxVertex.x)
			hull.maxVertex.x = actualDataC_->nodes[i].x;
		if (actualDataC_->nodes[i].y > hull.maxVertex.y)
			hull.maxVertex.y = actualDataC_->nodes[i].y;
		if (actualDataC_->nodes[i].z > hull.maxVertex.z)
			hull.maxVertex.z = actualDataC_->nodes[i].z;
	}

	actualDataC_->setHull(hull);

	updateData();
	createCoordinateSystem();
  //paintGL();

	return true;

}

bool SurfacePlot::loadFromData(TripleField const& data, CellField const& poly,  LineGroups const& groups)
{	
  actualData_p = actualDataC_;
	actualDataC_->nodes += data;
  actualDataC_->groups += groups;
	return true;
}	


