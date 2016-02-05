#ifndef PLOT_EXPORT_H
#define PLOT_EXPORT_H

#include "common_def.h"

#ifdef PLOT_STATICLIB
#define PLOT_API
#else
#ifdef PLOT_EXPORTS
#define PLOT_API SHARED_DECL_EXPORT
#else
#define PLOT_API SHARED_DECL_IMPORT
#endif
#endif

#endif
