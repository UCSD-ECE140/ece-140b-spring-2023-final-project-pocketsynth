# ECE 140B - Pocket Synth

Mark Muranov, Pierre Beurtheret, Nghi Lu, Marco Paredes

## Portable music making device!

This repo is made of two parts:

### MiniDAW

- MiniDAW is the software that runs on the Pocket Synth, allowing for standalone music creation. 

### PocketSynth Webapp

- This is the web server which runs remotely, allowing for PocketSynth's cloud connectivity.

- Projects are uploaded and downloaded via this server's REST API. The projects are stored as XML text documents in a SQLite database.