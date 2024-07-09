# Syncbox

This is syncbox that was designed to worked with as many devices as possible. It can sync TTL signals, wifi devices, motion capture cameras (e.g. Vicon), video cameras (using red LED). It can be daisy-chained with multiple devices or other syncboxes.

It has 2 operations modes:

1. Main: Receives a high TTL signal, then broadcasts a UDP packet (repeated 5 times for prevent packet loss) that can be captured by any WIFI enabled device. As long as the signal is high, the LEDs will be on. This allows to sync cameras. When the TTL signal goes low, a stop sync UDP packet is sent and the LEDs are turned off.
1. Receiver: When a box set to main mode broadcasts the sync UDP packet, the receiver turns on the LEDs and generates a TTL signal that allows syncing devices that are not WIFI enabled. When the sync end UDP packet is received, the TTL goes low and the LEDs turn off.
