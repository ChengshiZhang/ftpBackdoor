#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#ifdef _3DS
#include <3ds.h>
#endif
#include "console.h"
#include "ftp.h"


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

  console_print(CYAN "Welcome to the joke teller!\n\n" RESET);
  console_print(YELLOW "Press A to get a joke\n" RESET);
  console_print(GREEN "Press B twice to exit\n" RESET);
  
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
