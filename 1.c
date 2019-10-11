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

void mountain ()
{
  struct winsize w;
  ioctl (0, TIOCGWINSZ, &w);
  int height = 15;
  int width = w.ws_col;
  char grid[height][width];
  for (int y = 0; y < height; y++)
    {
      for (int x = 0; x < width; x++)
        {
          grid[y][x] = ' ';
        }
    }

  int elevation = 0;
  int dir = 1;
  for (int pos = 0; pos < width; pos++)
    {
      if (dir)
        {
          grid[elevation][pos] = '/';
          elevation++;
        }
      else
        {
          grid[elevation][pos] = '\\';

          elevation--;
        }
      if (elevation >= height - 1)
        {
          dir = 0;
        }
      if (elevation <= 0)
        {
          dir = 1;
        }
    }

  for (int y = height - 1; y >= 0; y--)
    {
      for (int x = 0; x < width; x++)
        {
          printf ("%c", grid[y][x]);
        }
      printf ("\n");
    }
}

int main (int argc, char **argv)
{
  mountain ();
  return 0;
}
