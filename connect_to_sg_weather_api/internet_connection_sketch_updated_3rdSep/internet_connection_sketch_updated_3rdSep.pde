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
int arrSize = readingsArr.size();
float weatherValue = 0.0;

// the following for loop is to determine the size of the array



for( int i = 0; i < arrSize ; i++)
{
  JSONObject stationInfo = readingsArr.getJSONObject(i);
  String stationIdCheck = stationInfo.getString("station_id");

  if( stationIdCheck.equals("S104"))
  {
    weatherValue = stationInfo.getFloat("value");
    break;
  }

}

println(weatherValue);

int weatherValue2 = round(weatherValue);
println(weatherValue2);
myPort.write(weatherValue2);

}
