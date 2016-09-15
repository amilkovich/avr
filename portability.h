#ifndef PORTABILITY_H
#define PORTABILITY_H

#define PORT_(port) PORT ## port
#define DDR_(port) DDR ## port
#define PIN_(port) PIN ## port

#define PORT(port) PORT_(port)
#define DDR(port) DDR_(port)
#define PIN(port) PIN_(port)

#endif
