export function Name() { return "Npad"; }
export function VendorId() { return 0xAA96; }
export function ProductId() { return 0xAAB4; }
export function Publisher() { return "Y&R"; }
export function Size() { return [4, 6]; }
export function DefaultPosition(){return [10, 100]; }
export function DefaultScale(){return 8.0}
/* global
shutdownColor:readonly
LightingMode:readonly
forcedColor:readonly
*/
export function ControllableParameters() {

    return [
        { "property":"bkset", "group": "lighting", "label":"BackLight Enable","type":"boolean","default":"true"},
        { "property":"ugset", "group": "lighting", "label":"Underglow Enable","type":"boolean","default":"true"},
        { "property": "LightingMode", "group": "lighting", "label": "Lighting Mode", "type": "combobox", "values": ["Canvas", "Forced"], "default": "Canvas" },
        { "property": "forcedColor", "group": "lighting", "label": "Forced Color", "min": "0", "max": "360", "type": "color", "default": "009bde" },
    ];
}

const vKeys =
    [
        0,  1,  2,  3,
        7,  6,  5,  4,
        8,  9,  10, 11,
        15, 14, 13, 12,
        16, 17, 18,
        20, 19
    ];

const vKeyNames =
    [
        "Head 1", "Head 2", "Head 3", "Head 4",
        "Num",    "/",      "*",      "-",
        "7",      "8",      "9",      "+",
        "4",      "5",      "6",      "ENT",
        "1",      "2",      "3",
                  "0",      "."
    ];
const vKeyPositions =
    [
        [0,0],[1,0],[2,0],[3,0],
        [0,1],[1,1],[2,1],[3,1],
        [0,2],[1,2],[2,2],[3,2],
        [0,3],[1,3],[2,3],[3,3],
        [0,4],[1,4],[2,4],
              [1,5],[2,5]
    ];

export function LedNames() 
{
    return vKeyNames;
}

export function LedPositions() 
{
    return vKeyPositions;
}

export function Initialize() 
{
    ClearReadBuffer();
    checkFirmwareType();
    versionQMK();
    versionSignalRGBProtocol();
    uniqueIdentifier();
    effectEnable();
}

export function Render() 
{
    sendColors();
}

export function Shutdown() 
{
    effectDisable();
}

function ClearReadBuffer(timeout = 10)
{
    let count = 0;
    let readCounts = [];
    device.flush();

    while(device.getLastReadSize() > 0){
        device.read([0x00], 64, timeout);
        count++;
        readCounts.push(device.getLastReadSize());
    }
    //device.log(`Read Count ${count}: ${readCounts} Bytes`)
}

function checkFirmwareType()
{
    let packet = [];
    packet[0] = 0x00;
    packet[1] = 0x28;

    device.write(packet, 64);

    let returnpacket = device.read(packet,64);
    let FirmwareTypeByte = returnpacket[2];

    if(FirmwareTypeByte !== 1 || FirmwareTypeByte !== 2)
    {
        device.notify("Unsupported Firmware: ", "Click Show Console, and then click on troubleshooting for your keyboard to find out more.", 0)
    }

    device.log("Firmware Type: " + FirmwareTypeByte);
    device.pause(30);
}

function versionQMK() //Check the version of QMK Firmware that the keyboard is running
{
    var packet = [];
    packet[0] = 0x00;
    packet[1] = 0x21;

    device.write(packet, 64);
    packet = device.read(packet,64);
    let QMKVersionByte1 = packet[2];
    let QMKVersionByte2 = packet[3];
    let QMKVersionByte3 = packet[4];
    device.log("QMK Version: " + QMKVersionByte1 + "." + QMKVersionByte2 + "." + QMKVersionByte3);
    device.pause(30);
}

function versionSignalRGBProtocol() //Grab the version of the SignalRGB Protocol the keyboard is running
{
    var packet = [];
    packet[0] = 0x00;
    packet[1] = 0x22;

    device.write(packet, 64);
    packet = device.read(packet,64);
    let ProtocolVersionByte1 = packet[2];
    let ProtocolVersionByte2 = packet[3];
    let ProtocolVersionByte3 = packet[4];
    device.log("SignalRGB Protocol Version: " + ProtocolVersionByte1 + "." + ProtocolVersionByte2 + "." + ProtocolVersionByte3);
    device.pause(30);
}

function uniqueIdentifier() //Grab the unique identifier for this keyboard model
{
    var packet = [];
    packet[0] = 0x00;
    packet[1] = 0x23;

    device.write(packet, 64);
    packet = device.read(packet,64);
    let UniqueIdentifierByte1 = packet[2];
    let UniqueIdentifierByte2 = packet[3];
    let UniqueIdentifierByte3 = packet[4];
    device.log("Unique Device Identifier: " + UniqueIdentifierByte1 + UniqueIdentifierByte2 + UniqueIdentifierByte3);
    device.pause(30);
}

function effectEnable() //Enable the SignalRGB Effect Mode
{
    let packet = [];
    packet[0] = 0x00;
    packet[1] = 0x25;

    device.write(packet,64);
    device.pause(30);
}

function effectDisable() //Revert to Hardware Mode
{
    let packet = [];
    packet[0] = 0x00;
    packet[1] = 0x26;

    device.write(packet,64);  
}

function grabColors(shutdown = false) 
{
	let rgbdata = [];

	for(let iIdx = 0; iIdx < vKeys.length; iIdx++)
	{
		let iPxX = vKeyPositions[iIdx][0];
		let iPxY = vKeyPositions[iIdx][1];
		let color;

		if(shutdown)
		{
			color = hexToRgb(shutdownColor);
		}
		else if (LightingMode === "Forced")
		{
			color = hexToRgb(forcedColor);
		}
		else
		{
			color = device.color(iPxX, iPxY);
		}

		let iLedIdx = vKeys[iIdx] * 3;
		rgbdata[iLedIdx] = color[0];
		rgbdata[iLedIdx+1] = color[1];
		rgbdata[iLedIdx+2] = color[2];
	}

	return rgbdata;
}

function sendColors()
{
	let rgbdata = grabColors();

    for(var index = 0; index < 8; index++) //This will need rounded up to closest value for your board.
    {
	let packet = [];
    let offset = index * 9;
	packet[0] = 0x00;
	packet[1] = 0x24;
	packet[2] = offset;
	packet[3] = 0x09;
	packet = packet.concat(rgbdata.splice(0, 27));
	device.write(packet, 64);

    }

}

function hexToRgb(hex) 
{
	let result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
	let colors = [];
	colors[0] = parseInt(result[1], 16);
	colors[1] = parseInt(result[2], 16);
	colors[2] = parseInt(result[3], 16);

	return colors;
}

export function Validate(endpoint) 
{
	return endpoint.interface === 1;
}

export function Image() 
{
	return "";
}