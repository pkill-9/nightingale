#ifndef _UTILS_H_
#define _UTILS_H_

/** constants that may be used to specify the scope of functions. */
#define PUBLIC
#define PRIVATE         static

/** define a bool type, and values for true and false. */
typedef int bool;

#define true            1
#define false           0

#define NULL            (void *) 0


bool isprintable (char character);


#endif /** _UTILS_H */

/** vim: set ts=4 sw=4 et : */
