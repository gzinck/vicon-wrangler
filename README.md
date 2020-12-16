# ViconWrangler

ViconWrangler is a simple C++ application that reads Vicon data using the
DataStream API and transmits it on WebSockets. This is useful when you want
to process the data with a language that is not typically supported by
Vicon.

## Development

Right now, ViconWrangler is under development---use at your own risk.
It currently only has the libraries for **macOS** support.

## Compiling and Running the ViconWrangler

1. Run `make`.
2. `cd` into `build` and run `./ViconWrangler [VICON_IP_ADDR]:801 [WEBSOCKET PORT]`
   where:
	- `[VICON_IP_ADDR]` is the IP address of the computer running the Vicon
      motion capture software (such as Tracker).
	- `[WEBSOCKET_PORT]` is the port the Vicon data is transmitted on using
	  websockets. Defaults to 3456.

## Testing ViconWrangler

The easiest way to see what's coming out of ViconWrangler is to use
`websocat`. You can install it with `brew install websocat` on macOS.
Then, while running ViconWrangler in a separate terminal window, run
`websocat ws://localhost:3456` to see the output.
