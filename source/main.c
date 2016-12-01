#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#ifdef _3DS
#include <3ds.h>
#endif
#include "console.h"
#include "ftp.h"

static int JokePointer = 0;
const char Joke1[] = "Can a kangaroo jump higher than a house? Of course, a house does not jump at all. \n \n";
const char Joke2[] = "Anton, do you think I am a bad mother?\nMy name is Paul.\n\n";
const char Joke3[] = "My dog used to chase people on a bike a lot. It got so bad, finally I had to take his bike away.\n \n";
const char Joke4[] = "A programmer is going to the grocery store and his wife tells him:\nBuy a gallon of milk, and if there are eggs, buy a dozen. \nSo the programmer goes, buys everything, and drives back to his house. \nUpon arrival, his wife angrily asks him, \nWhy did you get 13 gallons of milk? \nThe programmer says, \nThere were eggs!\n\n";

/*! looping mechanism
 *
 *  @param[in] callback function to call during each iteration
 *
 *  @returns loop status
 */
static loop_status_t
loop(loop_status_t (*callback)(void))
{
  loop_status_t status = LOOP_CONTINUE;

#ifdef _3DS
  while(aptMainLoop())
  {
    status = callback();
    console_render();
    if(status != LOOP_CONTINUE)
      return status;
  }

  return LOOP_EXIT;
#else
  while(status == LOOP_CONTINUE)
    status = callback();
  return status;
#endif
}

#ifdef _3DS
/*! wait until the B button is pressed
 *
 *  @returns loop status
 */
static loop_status_t
wait_for_b(void)
{
  /* update button state */
  hidScanInput();

  /* check if B was pressed */
  if(hidKeysDown() & KEY_B){
    return LOOP_EXIT;
  }

  else if(hidKeysDown() & KEY_A){
    if(JokePointer==0){
      console_print(Joke1);
      JokePointer++;
    }
    else if(JokePointer==1){
      console_print(Joke2);
      JokePointer++;
    }
    else if(JokePointer==2){
      console_print(Joke3);
      JokePointer++;
    }
    else if(JokePointer==3){
      console_print(Joke4);
      JokePointer=0;
    }
  }
  
  /* B was not pressed */
  return LOOP_CONTINUE;
}
#endif



/*! entry point
 *
 *  @param[in] argc unused
 *  @param[in] argv unused
 *
 *  returns exit status
 */
int
main(int  argc,
     char *argv[])
{
  loop_status_t status = LOOP_RESTART;

#ifdef _3DS
  /* initialize needed 3DS services */
  acInit();
  gfxInitDefault();
  gfxSet3D(false);
  sdmcWriteSafe(false);
#endif

  /* initialize console subsystem */
  console_init();
  console_set_status("\n" GREEN STATUS_STRING RESET);

  console_print(YELLOW "Press B to get started!\n" RESET);
  
  while(status == LOOP_RESTART)
  {
    /* initialize ftp subsystem */
    if(ftp_init() == 0)
    {
      /* ftp loop */
      status = loop(ftp_loop);

      /* done with ftp */
      ftp_exit();
    }
    else
      status = LOOP_EXIT;
  }

#ifdef _3DS
  u64 id;
  APT_GetProgramID(&id);

  if(id != 0x000400000BEEF500)
  {
    console_print("Press A Lemme tell u a joke\nPress B again to exit\n\n");
    loop(wait_for_b);
  }
  else
  {
    console_print("There is an error :[ \nPress the Home Button to exit\n");
    while(aptMainLoop())
      console_render();
  }

  /* deinitialize 3DS services */
  gfxExit();
  acExit();
#endif

  return 0;
}
