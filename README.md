# ViconWrangler

ViconWrangler is a simple C++ application that reads Vicon data using the
DataStream API and transmits it on WebSockets. This is useful when you want
to process the data with a language that is not typically supported by
Vicon.

## Development

Right now, ViconWrangler is under development---use at your own risk.
It currently only has the libraries for **macOS** support.

## Compiling and Running the ViconWrangler

1. Install dependencies using `make deps`. Running this will install
   the following:
    - OpenCV
2. Run `make`.
3. `cd` into `build` and run `./ViconWrangler [VICON_IP_ADDR]:801 [WEBSOCKET PORT]`
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
	"time": 1608134510614568, // received timestamp minus vicon latency
	// The type of information being sent
	"type": "frameInfo"
}
```

## Getting the Transformation Matrix

ViconWrangler sends coordinates in terms of the Vicon coordinate 
system. If you want to get coordinates in terms of some other
coordinate system (for instance, relative to a device's pixel
coordinates), you can do that by sending a message like the one below
via the WebSocket to ViconWrangler:

```javascript
{
	// A set of coordinates in terms of the Vicon coordinate system
	"source": [[0, 0, 0], [0, 10, 0], [10, 10, 0], [10, 0, 0]],
	// A coordesponding set of coordinates in terms of the desired
	// coordinate system. So, here, we'd be getting an affine
	// transformation matrix that converts [0, 0, 0] to [4, 8, 3],
	// [0, 10, 0] to [54, 68, 73], etc.
	"dest": [[4, 8, 3], [54, 68, 73], [64, 88, 103], [14, 28, 33]]
}
```

This returns a data structure like the following:

```javascript
{
	"result": [
		[1.0,5.0000000000000018,0.0,3.9999999999999876],
		[2.0000000000000009,6.0000000000000036,0.0,7.9999999999999796],
		[3.0000000000000009,7.0000000000000027,0.0,2.9999999999999751]
	],
	"type": "transformationMatrix"
}
```

This matrix can be used to convert from the Vicon coordinates to your
desired coordinate system.

Note that the implementation for transforming coordinates is part of
OpenCV4.
