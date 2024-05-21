/* CODIGO ELABORADO POR MIGUEL GENARO RODRIGUEZ GARCIA */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <conio.c>
#include <conio.h>

#define ENTER 13
#define KEY_V 118
#define ESC 27

#define RULETA_VALMAX 2000
#define RULETA_VALMIN 100
#define RULETA_QTY 3
#define RULETA_VALUES 5
#define RULETA_DEFAULT_IND 0

#define VERTICAL 1
#define HORIZONTAL 0
#define ARRIBA 0
#define ABAJO 1
#define ESPACIADO 6

#define POSX 5
#define POSY 6

#define CT BLUE
#define CF LIGHTGRAY
#define CTS RED
#define CFS YELLOW
#define CMT GREEN

void printRoulettes(int[], int, int, int, int, int, int);
void moveRoulettes(int[], int, int[RULETA_QTY], int, int, int, int, int);
void printPublicStats(int, int);
void endScreen();

int getBetAmount();
int getMovementDirection();
void genContent(int[], int);
void getStats(int[], int[], int, int);

void showRouletteXY(int[], int, int, int, int, int);
int moveRouletteXY(int[], int, int, int, int, int);

int itExists(int[], int, int);
int is100Multiple(int);
int randrange(int, int);
void setColor(int, int);
void colorDefault();

int main()
{
   srand(time(NULL));
   int money;
   int direction;
   int orientacion;
   int finally = 0;
   do
   {
      int initialDeposit;
      int agains = 3;
      int roulette[RULETA_VALUES];
      int tecla;

      system("cls");
      printf("Bienvenido a las apuestas: \n");

      direction = getMovementDirection();

      orientacion = VERTICAL;

      genContent(roulette, RULETA_VALUES);

      money = getBetAmount();
      do
      {
         // Inicializar variables de juego
         int preResult;

         int resultList[RULETA_QTY] = {0, 0, 0};
         int stats[2] = {0, 0}; // {sames, winnerValue}

         // Mostrar balance
         system("cls");
         printPublicStats(money, agains);

         /* MOSTRAR RULETAS INICIALES */
         gotoxy(POSX, POSY);
         printf("Presiona [ENTER] para poner a prueba tu suerte...");
         gotoxy(POSX, POSY + 1);
         printf("Presiona [v] para para cambiar la orientaci%cn de la ruleta.", 162);
         printRoulettes(roulette, RULETA_QTY, RULETA_VALUES, RULETA_DEFAULT_IND, POSX, POSY + 2, orientacion);

         /* LEER PRESIONADO DE TECLAS */
         do
         {
            tecla = getch();
         } while (tecla != ENTER && tecla != KEY_V);

         // TECLA V: CAMBIAR ORIENTACION
         if (tecla == KEY_V)
         {
            gotoxy(POSX, POSY + 1);
            setColor(BLACK, WHITE);
            printf("Presiona [v] para para cambiar la orientaci%cn de la ruleta.", 162);
            colorDefault();
            Sleep(200);

            if (orientacion == VERTICAL)
            {
               orientacion = HORIZONTAL;
            }
            else
            {
               orientacion = VERTICAL;
            }
         }

         // TECLA ENTER: GIRAR RULETAS
         if (tecla == ENTER)
         {
            gotoxy(POSX, POSY);
            setColor(BLACK, WHITE);
            printf("Presiona [ENTER] para poner a prueba tu suerte...");
            colorDefault();
            moveRoulettes(roulette, RULETA_QTY, resultList, RULETA_VALUES, POSX, POSY + 2, orientacion, direction);

            // Conseguir estadisticas al finalizar giro de ruleta
            getStats(resultList, stats, RULETA_QTY, money);

            preResult = getBetResult(stats[0], stats[1], RULETA_QTY, money);

            if (preResult == 0)
            {
               system("cls");
               printPublicStats(money, agains);
               gotoxy(POSX, POSY);
               setColor(WHITE, RED);
               printf("%cLo perdiste todo, mejor busca un trabajo!", 33);
               colorDefault();
               gotoxy(POSX, POSY + 2);
               if (agains > 0)
               {
                  // fflush(stdin);
                  printf("Pero... a%cn te quedan comodines disponibles.", 163);
                  int option;
                  do
                  {
                     gotoxy(POSX, POSY + 3);
                     printf("%cDeseas probar de nuevo tu suerte? [1] Si [0] No \n", 168);
                     scanf("%d", &option);
                  } while (option != 0 && option != 1);

                  if (option == 1)
                  {
                     agains -= 1;
                     preResult = money;
                  }
                  else
                  {
                     finally = 1;
                  }
               }
               else
               {
                  printf("Presionar [ENTER] para jugar de nuevo y [ESC] para salir.");
                  int presionado;
                  do
                  {
                     presionado = getch();
                  } while (presionado != ENTER && presionado != ESC);

                  if (presionado == ESC)
                     finally = 1;
               }
            }

            if (preResult >= 500)
            {
               gotoxy(POSX, POSY + RULETA_VALUES * 2);
               setColor(WHITE, CYAN);
               printf("Presiona cualquier tecla para seguir jugando...");
               getch();
               colorDefault();
            }

            money = preResult;
         }
      } while (money >= 500);
   } while (finally == 0);

   endScreen();
   return 0;
}

/*
   Función: int randrange
   Argumento: (int) liminf -> Limite inferior, (int) limsup -> Limite superior.
   Objetivo: Generar número aleatorio multiplo de 100 dentro de un rango.
   Retorno: Número generado.
*/
int randrange(int liminf, int limsup)
{
   return rand() % (limsup - liminf + 1) + liminf;
}

/*
   Función: int getBetAmount
   Argumento: Nada.
   Objetivo: Solicitar y validar monto para la apuesta.
   Retorno: (int) Monto introducido por el usuario.
*/
int getBetAmount()
{
   int amount;
   do
   {
      printf("Introduzca el monto a apostar (Valor m%cnimo RD$500): \n", 161);
      scanf("%d", &amount);

      if (amount < 500)
      {
         printf("El monto debe ser m%cnimo de RD$500.\n", 161);
      }

      if (!is100Multiple(amount))
      {
         printf("El monto debe ser m%cltiplo de 100 (Ej: 100, 300, 1000).\n", 163);
      }

   } while (amount < 500 && is100Multiple(amount) == 0);

   return amount;
}

/*
   Función: void getMovementDirection
   Argumento: Nada.
   Objetivo: Solicitar la dirección de giro de la ruleta.
   Retorno: (int) Dirección introducida por el usuario.
*/
int getMovementDirection()
{
   int mdirection;

   do
   {
      printf("Introduzca la direcci%cn de giro para la ruleta, (0) Arriba / Izquierda  y (1) Abajo / Derecha:\n", 162);
      scanf("%d", &mdirection);
   } while (mdirection != ARRIBA && mdirection != ABAJO);

   printf("La ruleta girar%c en direcci%cn %s. \n", 160, 162, mdirection == ARRIBA ? "arriba" : "abajo");
   return mdirection;
}

/*
   Función: void getStats
   Argumento: int resultList[] -> Lista de resultados del giro de ruletas, int stats[] -> Variable que almacena las estadisticas, int cant -> Cantidad de resultados, int money -> Dinero apostado.
   Objetivo: Conseguir estadisticas luego de girar la ruleta: combinaciones y numero repetido.
   Retorno: Nada.
*/
void getStats(int resultList[], int stats[], int cant, int money)
{

   int i, j;
   int sames = 0;
   int winnerValue = 0;

   for (i = 0; i < cant; i++)
   {
      for (j = i + 1; j < cant; j++)
      {
         if (resultList[i] == resultList[j])
         {
            winnerValue = resultList[i];
            sames++;
         }
      }
   }

   stats[0] = sames;
   stats[1] = winnerValue;
}

/*
   Función: int printPublicStats
   Argumento: int money -> Dinero del usuario, int agains -> Cantidad de comodines.
   Objetivo: Imprimir las estadisticas visibles para el usuario.
   Retorno: Nada.
*/
void printPublicStats(int money, int agains)
{
   gotoxy(POSX, POSY - 3);
   setColor(CMT, BLACK);
   printf("Tu actual monto es de RD$%d", money);
   gotoxy(POSX, POSY - 2);
   setColor(CYAN, BLACK);
   printf("%c%d", 208, agains);
   gotoxy(POSX + 3, POSY - 2);
   colorDefault();
   printf("(Comodines disponibles)");
}

/*
   Función: void endScreen
   Argumento: Nada.
   Objetivo: Mostrar pantalla del final del juego.
   Retorno: Nada.
*/
void endScreen()
{
   system("cls");
   gotoxy(POSX, POSY - 2);
   setColor(WHITE, BLUE);
   printf("Gracias por jugar.");
   colorDefault();
}

/*
   Función: int getBetResult
   Argumento: int sames -> Resultados en comun, int winnerValue -> Numero ganado(s), int cant -> Cantidad de resultados, int money -> Dinero apostado.
   Objetivo: Calcular los resultados finales de cara a la apuesta.
   Retorno: (int) Cantidad de dinero ganada o perdida en base a los resultados.
*/
int getBetResult(int sames, int winnerValue, int cant, int money)
{
   // Premio: Si solo hay 2 iguales
   if (sames == 1)
   {
      return winnerValue + (money * 2);
   }
   else if (sames > 1) // Premio: Si solo hay 3 iguales.
   {
      return ((winnerValue * cant) * 2) + (money * 4);
   }
   else
   {
      return 0;
   }
}

/*
   Función: void genContent
   Argumento: int lista[] -> Arreglo, int cant -> Cantidad de elementos.
   Objetivo: Solicitar y validar monto para la apuesta.
   Retorno: (int) Monto introducido por el usuario.
*/
void genContent(int lista[], int cant)
{
   int i = 0;
   int val;
   while (i < cant)
   {
      val = randrange(RULETA_VALMIN / 100, RULETA_VALMAX / 100) * 100;
      if (!itExists(lista, i, val))
      {
         lista[i++] = val;
      }
   }
}

/*
   Función: void printRoulettes
   Argumento: int lista[] -> Arreglo, int cant -> Cantidad de elementos, int sel -> Elemento seleccionado, int posx -> Posicion X, int posy -> Posicion Y, int orientacion -> Orientacion de la ruleta.
   Objetivo: Mostrar ruleta con valores suministrados.
   Retorno: Nada.
*/
void printRoulettes(int roulette[], int rqty, int cant, int sel, int posx, int posy, int orientation)
{
   if (orientation == VERTICAL)
   {
      int i;
      for (i = 0; i < rqty; i++)
      {
         showRouletteXY(roulette, RULETA_VALUES, RULETA_DEFAULT_IND, posx + ESPACIADO * (2 + i), posy + 2, orientation);
      }
   }
   else if (orientation == HORIZONTAL)
   {
      int i;
      for (i = 0; i < rqty; i++)
      {
         showRouletteXY(roulette, RULETA_VALUES, RULETA_DEFAULT_IND, posx + ESPACIADO, posy + 2 + i, orientation);
      }
   }
}

/*
   Función: void moveRoulettes
   Argumento: int roulette[] -> Arreglo, int rqty -> Cantida de ruletas, int resultList[] -> Lista de resultados del giro de ruletas, int cant -> Cantidad de elementos, int posx -> Posicion X, int posy -> Posicion Y, int orientacion -> Orientacion de la ruleta, int direction -> Dirección.
   Objetivo: Hacer girar ruletas con valores suministrados.
   Retorno: Nada.
*/
void moveRoulettes(int roulette[], int rqty, int resultList[RULETA_QTY], int cant, int posx, int posy, int orientation, int direction)
{
   if (orientation == VERTICAL)
   {
      int i;
      for (i = 0; i < rqty; i++)
      {
         resultList[i] = moveRouletteXY(roulette, cant, posx + ESPACIADO * (2 + i), posy + 2, orientation, direction);
      }
   }
   else if (orientation == HORIZONTAL)
   {
      int i;
      for (i = 0; i < rqty; i++)
      {
         resultList[i] = moveRouletteXY(roulette, cant, posx + ESPACIADO, posy + 2 + i, orientation, direction);
      }
   }
}

/*
   Función: void showRouletteXY
   Argumento: int lista[] -> Arreglo, int cant -> Cantidad de elementos, int sel -> Elemento seleccionado, int posx -> Posicion X, int posy -> Posicion Y, int orientacion -> Orientacion de la ruleta.
   Objetivo: Mostrar ruleta con valores suministrados.
   Retorno: Nada.
*/
void showRouletteXY(int lista[], int cant, int sel, int posx, int posy, int orientacion)
{
   _setcursortype(0);
   int ind;
   for (ind = 0; ind < cant; ind++)
   {
      if (orientacion == VERTICAL)
         gotoxy(posx, posy + ind);
      else
         gotoxy(posx + ind * ESPACIADO, posy);

      setColor(CT, CF);
      if (ind == sel)
         setColor(CTS, CFS);

      printf("%*d", ESPACIADO, lista[ind]);
   }
   colorDefault();
}

/*
   Función: int moveRouletteXY
   Argumento: int lista[] -> Arreglo, int cant -> Cantidad de elementos, int posx -> Posicion X, int posy -> Posicion Y, int orientacion -> Orientacion de la ruleta.
   Objetivo: Mover la ruleta de manera aleatoria.
   Retorno: Nada.
*/
int moveRouletteXY(int lista[], int cant, int posx, int posy, int orientacion, int direction)
{
   int timesMax = randrange((RULETA_VALMIN + 200) / 100, (RULETA_VALMAX - 1000) / 100);
   int selectedEl;

   int times = 0;
   do
   {
      int startIndex = times == timesMax - 1 ? randrange(0, cant - 1) : 0;
      selectedEl = 0;
      int counter;
      for (counter = startIndex; counter < cant; counter++)
      {
         showRouletteXY(lista, cant, selectedEl, posx, posy, orientacion);
         Sleep(150);

         if (counter == cant - 1 && times == timesMax - 1)
         {
            continue;
         }
         if (direction == ARRIBA)
         {
            if (selectedEl == 0)
            {
               selectedEl = cant - 1;
            }
            else
            {
               selectedEl--;
            }
         }
         else if (direction == ABAJO)
         {
            if (selectedEl == cant - 1)
            {
               selectedEl = 0;
            }
            else
            {
               selectedEl++;
            }
         }

         /*gotoxy(posx, posy + cant + 2);
         printf("%d", selectedEl);
         gotoxy(posx, posy + cant + 3);
         printf("%d", counter);*/
      }
      times++;
   } while (times < timesMax);
   Sleep(500);
   return lista[selectedEl];
}

/*
   Función: int itExists
   Argumento: int lista[] -> Arreglo, (int) cant -> Cantidad de elementos.
   Objetivo: Saber si el valor existe en el arreglo suministrado.
   Retorno: (int) 1 -> Existe, (int) 0 -> No existe.
*/
int itExists(int lista[], int cant, int val)
{
   int i;
   for (i = 0; i < cant; i++)
      if (lista[i] == val)
         return 1;

   return 0;
}

/*
   Función: int Multiplo100
   Argumento: int num -> Número suministrado.
   Objetivo: Saber si el valor es multiplo de 100.
   Retorno: (int) 1 -> Sí lo es, (int) 0 -> No lo es.
*/
int is100Multiple(int num)
{
   return num % 100 == 0;
}

/*
   Función: void setColor
   Argumento: (int) ct -> Color del texto, (int) cf -> Color del fondo.
   Objetivo: Asignar color de fondo y texto del programa.
   Retorno: Nada.
*/
void setColor(int ct, int cf)
{
   textbackground(cf);
   textcolor(ct);
}

/*
   Función: void colorDefault
   Argumento: Nada.
   Objetivo: Asignar los colores de texto y fondo por defecto en el programa.
   Retorno: Nada.
*/
void colorDefault()
{
   setColor(WHITE, BLACK);
}
