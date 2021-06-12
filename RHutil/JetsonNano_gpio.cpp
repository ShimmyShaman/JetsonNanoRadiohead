
#include <stdlib.h>
#include <stdio.h>
// #include <errno.h>
#include <unistd.h>
#include <fcntl.h>
// #include <poll.h>
#include <string.h>

/****************************************************************
 * Constants
 ****************************************************************/

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define POLL_TIMEOUT (3 * 1000) /* 3 seconds */
#define MAX_BUF 64

/****************************************************************
 * gpio_export
 ****************************************************************/
int gpio_export(unsigned int gpio)
{
	int fd, len, err;
	char buf[MAX_BUF];

	fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
	if (fd < 0)
	{
		perror("gpio/export");
		return fd;
	}
	printf("gpio_export(%u)\n", gpio);

	len = snprintf(buf, sizeof(buf), "%d", gpio);
	err = write(fd, buf, len);
	close(fd);

	if (err < 0)
	{
		perror("gpio_export : Pin probably already exported ");
	}

	return err < 0 ? err : 0;
}

/****************************************************************
 * gpio_unexport
 ****************************************************************/
int gpio_unexport(unsigned int gpio)
{
	int fd, len, err;
	char buf[MAX_BUF];

	fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
	if (fd < 0)
	{
		perror("gpio/unexport");
		return fd;
	}
	printf("gpio_unexport(%u)\n", gpio);

	len = snprintf(buf, sizeof(buf), "%d", gpio);
	err = write(fd, buf, len);
	close(fd);
	return err < 0 ? err : 0;
}

/****************************************************************
 * gpio_set_dir
 ****************************************************************/
int gpio_set_dir(unsigned int gpio, unsigned int out_flag)
{
	int fd, err;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/direction", gpio);

	fd = open(buf, O_WRONLY);
	if (fd < 0)
	{
		perror("gpio/direction");
		printf("--buf:'%s'\n", buf);
		//exit(44);
		return fd;
	}
#ifdef VERBOSE
	printf("gpio_set_dir(%u, %s)\n", gpio, out_flag ? "out" : "in");
#endif

	if (out_flag)
		err = write(fd, "out", 4);
	else
		err = write(fd, "in", 3);

	close(fd);
	return err < 0 ? err : 0;
}

/****************************************************************
 * gpio_set_value
 ****************************************************************/
int gpio_set_value(unsigned int gpio, unsigned int value)
{
	int fd, err;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

	fd = open(buf, O_WRONLY);
	if (fd < 0)
	{
		perror("gpio/set-value");
		printf("--buf:'%s'\n", buf);
		//exit(45);
		return fd;
	}
#ifdef VERBOSE
	printf("gpio_set-value(%u, %u)\n", gpio, value);
#endif

	if (value)
		err = write(fd, "1", 2);
	else
		err = write(fd, "0", 2);

	close(fd);
	return err < 0 ? err : 0;
}

/****************************************************************
 * gpio_get_value
 ****************************************************************/
int gpio_get_value(unsigned int gpio, unsigned int *value)
{
	int fd, err;
	char buf[MAX_BUF];
	char ch;

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

	fd = open(buf, O_RDONLY);
	if (fd < 0)
	{
		perror("gpio/get-value");
		printf("--buf:'%s'\n", buf);
		//exit(46);
		return fd;
	}

	err = read(fd, &ch, 1);
	if (err > 0)
	{
		if (ch != '0')
		{
			*value = 1;
		}
		else
		{
			*value = 0;
		}
	}
#ifdef VERBOSE
	printf("gpio_get-value(%u, ->%u)\n", gpio, *value);
#endif

	close(fd);
	return 0;
}

/****************************************************************
 * gpio_set_edge
 ****************************************************************/

int gpio_set_edge(unsigned int gpio, char *edge)
{
	int fd, err;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);

	fd = open(buf, O_WRONLY);
	if (fd < 0)
	{
		perror("gpio/set-edge");
		printf("--buf:'%s'\n", buf);
		return fd;
	}

	err = write(fd, edge, strlen(edge) + 1);
	close(fd);
	return err < 0 ? err : 0;
}

/****************************************************************
 * gpio_fd_open
 ****************************************************************/

int gpio_fd_open(unsigned int gpio)
{
	int fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

	fd = open(buf, O_RDONLY | O_NONBLOCK);
	if (fd < 0)
	{
		perror("gpio/fd_open");
		printf("--buf:'%s'\n", buf);
	}
	return fd;
}

/****************************************************************
 * gpio_fd_close
 ****************************************************************/

int gpio_fd_close(int fd)
{
	return close(fd);
}

void ensure_export(unsigned int gpio)
{
  int r = 9, am = 1600;
  while (gpio_export(gpio) && r > 0)
  {
    --r;
    usleep(am);
    am *= 2;
  }
  if (!r)
  {
    perror("ensure_export");
    //exit(44);
  }
}

void ensure_set_dir(unsigned int gpio, unsigned int out_flag)
{
  int r = 9, am = 1600;
  while (gpio_set_dir(gpio, out_flag) && r > 0)
  {
    --r;
    usleep(am);
    am *= 2;
  }
  if (!r)
  {
    perror("ensure_set_dir");
    //exit(45);
  }
}

void ensure_set_value(unsigned int gpio, unsigned int value)
{
  int r = 10, am = 1600;
  while (gpio_set_value(gpio, value) && r > 0)
  {
    --r;
    usleep(am);
    am *= 2;
  }
  if (!r)
  {
    // perror("ensure_set_value");
    //exit(46);
  }
}