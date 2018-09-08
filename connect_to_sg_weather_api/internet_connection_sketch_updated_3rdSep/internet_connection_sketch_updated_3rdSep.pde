import processing.serial.*;

// The serial port:
Serial myPort;

void setup() {
//List all available serial ports:
println(Serial.list());
myPort = new Serial(this, Serial.list()[0], 9600);


}

void draw() {
JSONObject weatherDataPage = loadJSONObject("https://api.data.gov.sg/v1/environment/air-temperature"); //reads text on the url;
JSONArray itemArr = weatherDataPage.getJSONArray("items");
JSONObject innerItemObj = itemArr.getJSONObject(0);
JSONArray readingsArr = innerItemObj.getJSONArray("readings");
JSONObject stationInfo = readingsArr.getJSONObject(1);
float weatherValue = stationInfo.getFloat("value");
println(weatherValue);

int weatherValue2 = round(weatherValue*10);
println(weatherValue2);
myPort.write(weatherValue2);

}
