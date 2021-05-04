#include <NGP_GamePad.h>
#include <stdio.h>
int main() {
  NGP_Initialize();

  int num_gamepads = NGP_NumGamePads();
  printf("Found %d game pads\n", num_gamepads);
  for (int i = 0; i < num_gamepads; i++) {
    NGP_GamePad* p = NGP_GamePadOpen(i);
    printf("%s\n", NGP_GamePadName(p));
    NGP_GamePadFree(p);
  }

  return 0;
}