#include        <stdio.h>
#include        <math.h>
#include        <matrix.h>
#include        <matrix3.h>
#include		<stdlib.h>


#define errmesg(mesg)   printf("Error: %s error: line %d\n",mesg,__LINE__)
#define space printf("\n");


void main()
{
   int i,j;
   
   MAT *A = MNULL, *B = MNULL, *C = MNULL, *D = MNULL, *E = MNULL;
   VEC *x = VNULL, *y = VNULL, *z = VNULL;
   double k;
   
   Real *buffer;
   int ret=-1;

   void *pMem;


/****** Выделение памяти под матрицы и вектора **************/
   

   mem_info_on(TRUE);

   mem_info();

   // выделение памяти одним куском под все матрицы и вектора
   pMem = calloc(1,3*SET_MAT_SIZE(2,2) + 2*SET_MAT_SIZE(3,3) + 3*SET_VEC_SIZE(2));

   // инициализация матриц и векторов
   mat_get(&A, pMem, 2, 2);
	pMem = pshift(pMem, 2,2);
   mat_get(&B, pMem, 2, 2);
    pMem = pshift(pMem, 2,2);
   mat_get(&C, pMem, 2, 2);
	pMem = pshift(pMem, 2,2);
   mat_get(&D, pMem, 3, 3);
	pMem = pshift(pMem, 3,3);
   mat_get(&E, pMem, 3, 3);
    pMem = pshift(pMem, 3,3);
   vec_get(&x, pMem, 2);
    pMem = pshift(pMem, 3,0);
   vec_get(&y, pMem, 2);
    pMem = pshift(pMem, 3,0);
   vec_get(&z, pMem, 2);
    pMem = pshift(pMem, 3,0);

   // строка, столбец
   A -> me [0][0] = 1.0;
   A -> me [0][1] = 8.0;
   A -> me [1][0] = 4.0;
   A -> me [1][1] = 3.0;

   B -> me [0][0] = 3.0;
   B -> me [0][1] = 10.0;
   B -> me [1][0] = 23.8;
   B -> me [1][1] = 32.5;

   D -> me [0][0] = 21.4;
   D -> me [0][1] = 46.32;
   D -> me [0][2] = 43.8;
   D -> me [1][0] = 756.21;
   D -> me [1][1] = 543.8;
   D -> me [1][2] = 231.87;
   D -> me [2][0] = 321.5;
   D -> me [2][1] = 765.7;
   D -> me [2][2] = 343.3;

   x -> ve [0] = 3.0;
   x -> ve [1] = 4.0;

   y -> ve [0] = 1.0;
   y -> ve [1] = 2.0;

   v_rand(z);
   m_rand(C);
   m_rand(E);

   mem_info();


   
   /***************** Печать заданных матриц и векторов ******************/

   printf ("A:\n");
   m_print(A);

   printf ("B:\n");
   m_print(B);

   printf ("C:\n");
   m_print(C);

   printf ("D:\n");
   m_print(D);

   printf ("E:\n");
   m_print(E);

   printf ("x:\n");
   v_print(x);

   printf ("y:\n");
   v_print(y);

   printf ("z:\n");
   v_print(z);

/**** Мат. операции над матрицами и вывод результатов на экран ******/

   m_transp(A, A);          //Транспонирование А
   printf ("A^T:\n");
   m_print(A);

   m_add(A, B, C);          //Сложение A+B, результат записывается в C
   printf ("A+B:\n");
   m_print(C);

   m_sub(A, B, C);          //Вычитание A-B, результат записывается в С
   printf ("A-B:\n");
   m_print(C);

   m_mlt(A,B,C);            // Умножение матрицу на матрицу A*B
   printf ("A*B:\n");
   m_print(C);

   mmtr_mlt(A, B, C);       //Умножение A*B^T
   printf ("A*B^T:\n");
   m_print(C);

   mtrm_mlt(C, A, B);       //Умножение A^T*B
   printf ("A^T*B:\n");
   m_print(C);

   mv_mlt(A, x, z);         //Умножение матрицы на вектор A*x, результат записывается в y
   printf ("A*y:\n");
   v_print(z);

   sm_mlt(10, A, C);        //Умножение матрицы на скаляр
   printf ("10*A:\n");
   m_print(C);

   vm_mlt(A, x, z);         //Умножение вектора на матрицу x*A, результат записывается в z
   printf ("x*A:\n");
   v_print(z);

   ms_mltadd(A, B, 40.0, C);   //Умножение на скаляр и сложение A + 40.0*B
   printf ("A + 40.0*B:\n");
   m_print(C);


   m_inverse(A, C);            //Нахождение обратной матрицы A^-1 // Скорее всего, тут фунция выделяет память под вектор и 
   printf ("A^-1:\n");         //не освобождает ее
   m_print(C);


   m_mlt(A,C,B);               //Проверка правильности нахождения обратной матрицы(должна получиться единичная матрица)
   printf ("A*A^-1:\n");
   m_print(B);

   m_mlt(C,A,B);               //Проверка правильности нахождения обратной матрицы(должна получиться единичная матрица)
   printf ("A^-1*A:\n");
   m_print(B);

   E = _m_copy(D, E, 1, 1);    //Копирование матрицы D в E
   printf ("E:\n");
   m_print(E);

   m_inverse(D, E);            //Обращение матрицы
   printf ("D:\n");
   m_print(D);

   sv_mlt(10,x,z);             //Умножение вектора на скаляр
   printf ("10*x:\n");
   v_print(z);


   v_add(x,y,z);
   printf ("x+y:\n");        //Сложение двух векторов x+y
   v_print(z);

   v_sub(x,y,z);
   printf ("x-y:\n");          //Вычитание x-y
   v_print(z);

   k = in_prod(x,y);           //Скалярное произведение векторов
   printf("x*y   %15.8e",k);
   printf("\n");

   m_ident(A);                 //Преобразует матрицу в единичную
   printf ("A:\n");
   m_print(A);

	space
	space

/******* Обработка исключений, напр., ошибка E_NULL, E_SIZES и т.д. ************/

   //catch(E_NULL,
	  //catchall(v_add(VNULL,VNULL,VNULL);
		 //    errmesg("tracecatch() failure"),
		 //    printf("# tracecatch() caught error\n");
		 //    error(E_NULL,"main"));
	  //           errmesg("catch() failure"),
	  //printf("# catch() caught E_NULL error\n"));

   //catch(E_SIZES,
	  //catchall(v_add(A,D,D);
		 //    errmesg("tracecatch() failure"),
		 //    printf("# tracecatch() caught error\n");
		 //    error(E_SIZES,"main"));
	  //           errmesg("catch() failure"),
	  //printf("# catch() caught E_SIZES error\n"));   


/*********** Освобождение памяти  ***********/

   
   getchar();
}
