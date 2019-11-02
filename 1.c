#include <stdio.h>
#include <sys/ioctl.h>
#include <poll.h>

void args (int argc, char **argv)
{
  for (; *argv != NULL; argv++)
    {
      printf ("%s\n", *argv);
    }
}

void star ()
{
  int pos = 0;
  int dir;
  while (1)
    {
      if (pos == 20)
        {
          dir = 0;
        }
      if (pos == 0)
        {
          dir = 1;
        }
      if (dir)
        {
          pos++;
        }
      else
        {
          pos--;
        }
      fprintf (stderr, "\r");
      for (int i = 0; i < pos; i++)
        {
          fprintf (stderr, " ", i);
        }
      fprintf (stderr, "*");
      for (int i = pos + 1; i <= 20; i++)
        {
          fprintf (stderr, " ", i);
        }
      poll (NULL, 0, 30);
    }
}

void fix ()
{
  int i, n = 20;
  for (i = 0; i < n; n--)
    (void) printf ("-");
  putchar ('\n');
}

void args2 (int argc, char **argv)
{
  for (; *argv != NULL; argv++)
    {
      printf ("%c\n", *(*(argv) + 1));
    }
}

int
drawMountain(int startPos,
             int startElevation,
             int length,
             int summit,
             int summit_length,
             size_t grid_height,
             size_t grid_width,
             char grid[grid_height][grid_width]) {
  int elevation = startElevation;
  int dir = 1;
  for (int pos = startPos; pos <= startPos + length; pos++) {
    if (elevation == summit) {
      if (summit_length > 0) {
        dir = 0;
      } else {
        dir = -1;
        elevation--;
      }
    }

    if (elevation < 0) {
      return -pos;
    }

    if (dir == 1) {
      grid[elevation][pos] = '/';
      elevation++;
    } else if (dir == -1) {
      grid[elevation][pos] = '\\';
      elevation--;
    } else {
      grid[elevation][pos] = '_';
      summit_length--;
    }
  }
  return elevation;
}

void mountain ()
{
  struct winsize w;
  ioctl (0, TIOCGWINSZ, &w);
  int height = 15;
  int width = 100;
  char grid[height][width];
  for (int y = 0; y < height; y++)
    {
      for (int x = 0; x < width; x++)
        {
          grid[y][x] = ' ';
        }
    }
  drawMountain(0, 0, 7, 5, 0, height, width, grid);
  drawMountain(9, 2, 15, 8, 2, height, width, grid);
  for (int y = height - 1; y >= 0; y--)
    {
      for (int x = 0; x < width; x++)
        {
          printf ("%c", grid[y][x]);
        }
      printf ("\n");
    }
}
