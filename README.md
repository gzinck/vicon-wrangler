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
      motion capture software (such as Tracker). Make sure that computer is
	  already running the Vicon software before starting.
	- `[WEBSOCKET_PORT]` is the port the Vicon data is transmitted on using
	  websockets. Defaults to 3456.

## Testing ViconWrangler

The easiest way to see what's coming out of ViconWrangler is to use
`websocat`. You can install it with `brew install websocat` on macOS.
Then, while running ViconWrangler in a separate terminal window, run
`websocat ws://localhost:3456` to see the output.

## ViconWrangler Data Output

Every frame, a message is sent using WebSockets on the given port. The
message is formatted as a JSON string similar to below:

```javascript
{
	"Active Wand (Origin Tracking)": {
		// (x, y, z) coordinates for each marker
		"MarkerA": [0.0,0.0,0.0],
		"MarkerB": [0.0,0.0,0.0],
		"MarkerC": [0.0,0.0,0.0],
		"MarkerD": [0.0,0.0,0.0],
		"MarkerE": [0.0,0.0,0.0]
	},
	"frameno": 124562,
	// Epoch time
	"time": 1608134510614568 // received timestamp minus vicon latency
}
```
