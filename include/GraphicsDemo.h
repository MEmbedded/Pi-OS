#ifndef KERNEL_GRAPHICSDEMO_H
#define KERNEL_GRAPHICSDEMO_H

/**
 * @file GraphicsDemo.h
 * @brief Graphics demonstration routines for the Pi-OS kernel.
 */

/**
 * @brief Runs the graphics demonstration sequence.
 *
 * Executes a series of graphical operations to demonstrate the
 * framebuffer and rendering capabilities of the system.
 *
 * @return 0 on success, non-zero on failure.
 */
int playDemo();

#endif //KERNEL_GRAPHICSDEMO_H