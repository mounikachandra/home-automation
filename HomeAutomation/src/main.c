#include<SPI.h>
#include<Ethernet.h>
byte mac[]={0x90,0xA2,0xDA,0x0F,0x3E,0xD8};
IPAddress ip(192,168,4,70);
EthernetServer server(80);
String HTTP_req;
float temp=A2;
int on=7;
int tempin=8;
int tempde=6;
float val=0;
float val1;
int relay_status=0;
float i;
float j;
float temp_enter;
//int count1=0;
//int count2=0;
void setup()
{
Ethernet.begin(mac,ip);//ethernet connection
server.begin();//begins the server
Serial.begin(9600);//for serial communication
pinMode(temp,INPUT);//temperature sensor input
pinMode(on,OUTPUT);//relay for ac
pinMode(tempin,OUTPUT);//to increase the temperature
pinMode(tempde,OUTPUT);//to decrease the temperature
}
void loop()
{
EthernetClient client =server.available();//creates a variable
if(client)//checks whether there is any client or not
{
boolean currentLineBlank = true;
while(client.connected())//client connected
{
if(client.available())//client available
{
char c= client.read();//reading 1 byte from
HTTP_req +=c;//saving 1 byte in HTTP
//sending header response
if(c=='\n'&& currentLineBlank)//response header from the server
{
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("Connection: keep-alive");
// client.println("Refresh:5");
client.println();
//send data to web page that could be seen from client side
if(HTTP_req.indexOf("temp_button")>-1)
{
getButtonCode(client);
}
else if(HTTP_req.indexOf("temp_on")>-1)
{
getButtonRelay(client);
}
else if(HTTP_req.indexOf("temp_inc")>-1)
{
getButtonTempIn(client);
}
else if(HTTP_req.indexOf("temp_dec")>-1)
{
getButtonTempDe(client);
}
else
{
client.println("<!DOCTYPE html>");
client.println("<html>");
client.println("<head>");
//client.println("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
client.println("<style>");
// client.println("body { ");
// client.println("background-image:url('file:///G:/site/style/food.ping');");
// client.println("}");
client.println("h1 {text-align:center;}");
client.println("</style>");
client.println("<title>HOME AUTOMATION</title>");
client.println("<script>");
client.println("function getButtonCode(){");
client.println("var request1=new XMLHttpRequest();");
client.println("request1.onreadystatechange=function(){");
client.println("if(this.readyState==4){");
client.println("if(this.status==200){");
client.println("if(this.responseText!=null){");
client.println("document.getElementById(\"temp_status\").innerHTML=this.responseText;");
client.println("}}}}");
client.println("request1.open(\"GET\",\"temp_button\",true);");
client.println("request1.send(null);");
client.println("}");
client.println("</script>");
client.println("<script>");
client.println("function getButtonRelay(){");
client.println("var request4=new XMLHttpRequest();");
client.println("request4.onreadystatechange=function(){");
client.println("if(this.readyState==4){");
client.println("if(this.status==200){");
client.println("if(this.responseText!=null){");
client.println("document.getElementById(\"temp_status\").innerHTML=this.responseText;");
client.println("}}}}");
client.println("request4.open(\"GET\",\"temp_on\" ,true);");
client.println("request4.send(null);");
client.println("}");
client.println("</script>");
client.println("<script>");
client.println("function getButtonTempIn(){");
client.println("var request2=new XMLHttpRequest();");
client.println("request2.onreadystatechange=function(){");
client.println("if(this.readyState==4){");
client.println("if(this.status==200){");
client.println("if(this.responseText!=null){");
client.println("document.getElementById(\"temp_status\").innerHTML=this.responseText;");
client.println("}}}}");
client.println("request2.open(\"GET\",\"temp_inc\" ,true);");
client.println("request2.send(null);");
client.println("}");
client.println("</script>");
client.println("<script>");
client.println("function getButtonTempDe(){");
client.println("var request3=new XMLHttpRequest();");
client.println("request3.onreadystatechange=function(){");
client.println("if(this.readyState==4){");
client.println("if(this.status==200){");
client.println("if(this.responseText!=null){");
client.println("document.getElementById(\"temp_status\").innerHTML=this.responseText;");
client.println("}}}}");
client.println("request3.open(\"GET\",\"temp_dec\" ,true);");
client.println("request3.send(null);");
client.println("}");
client.println("</script>");
/* client.println("<script>");
client.println("function formdata() {");
client.println("var temp_enter=document.getElementById(\"tempinput\").value;");
client.println("document.writeln(\"temperature\"+temp_enter); }");
client.println("</script>");
*/
client.println("</head>");
client.println("<body bgcolor=\"#fae14c\">");//starting and closing body tags
client.println("<h1>TEMPERATURE CONTROL</h1>");
client.println("<p id=\"temp_status\">temperature not requested</p>");
//client.println("<pre>");
client.println( "<button type=\"button\" onclick=\"getButtonCode()\">temperature</button><br>");
/* client.println("<form>");
client.println("<input type=\"text\" id=\"tempinput\">");
client.println("<input type=\"submit\" value=\"submit\" onclick=\"formdata()\">");
client.println("</form>");
*/
client.println("<button type=\"button\" onclick=\"getButtonRelay()\">ON/OFF</button><br>");
client.println("<button type=\"button\" onclick=\"getButtonTempIn()\">tempincrease</button><br>");
client.println("<button type=\"button\" onclick=\"getButtonTempDe()\">tempdecrease</button><br>");
// client.println("<form method=\"get\">");
// futherOperations(client);
// client.println("</form>");
//client.println("</pre>");
client.println("</body>");
client.println("</html>");//closing html tag
}
Serial.println(HTTP_req);
HTTP_req="";//end the client statement
break;
}
if(c=='\n')
{
currentLineBlank=true;//end of a string
}
else if(c!='\r')
{
currentLineBlank=false;//not an end of the string
}
}//client.available();
}//client.connected();
delay(100);
client.stop();
}//client
}
/*void futherOperations(EthernetClient cl)
{
if(HTTP_req.indexOf("relay=1")>-1)
{// checking whether the box is checked in or not
if(relay_status)
{
relay_status=0;
}
else
{
relay_status=1;
}
}
if(relay_status)
{
digitalWrite(relay,HIGH);
cl.println("<input type=\"checkbox\" name=\"relay\" value=\"1\" onclick=\"submit();\"checked>relay");//relay is on
}
else
{
digitalWrite(relay,LOW);
cl.println("<input type=\"checkbox\" name=\"relay\" value=\"1\" onclick=\"submit();\">relay");//relay is off
}
}*/
void getButtonCode(EthernetClient cl1)
{
tempread(cl1);
}
void tempread(EthernetClient cl2)
{
val=analogRead(temp);
val1=(val*500.0)/1024.0;
cl2.println(" temperature = ");
cl2.println(val1);
}
void getButtonRelay(EthernetClient cl7)
{
relay(cl7);
}
void relay(EthernetClient cl8)
{
digitalWrite(on,HIGH);
cl8.println("button is pressed");
delay(500);
digitalWrite(on,LOW);
}
void getButtonTempIn(EthernetClient cl3)
{
tempincrease(cl3);
}
void tempincrease(EthernetClient cl4)
{
digitalWrite(tempin,HIGH);
cl4.println("increasebutton is pressed");
delay(500);
digitalWrite(tempin,LOW);
//count1++;
//cl4.println("button is pressed");
//cl4.println(count1);
}
/*void tempincrease(EthernetClient cl4)
{
if((temp_enter-val1)>0)
{
for(i=0;i<(temp_enter-val1);i++)
{
digitalWrite(tempin,HIGH);
cl4.println("tempincrease is made high");
delay(300);
digitalWrite(tempin,LOW);
delay(2000);
cl4.println("tempincreased is made low");
}
}
// digitalWrite(tempin,HIGH);
// cl4.println("tempincrease is PRESSED");
}
*/
void getButtonTempDe(EthernetClient cl5)
{
tempdecrease(cl5);
}
/*void tempdecrease(EthernetClient cl6)
{
if((val1-temp_enter)>0.0)
{
for(j=16.0;j<(val1-temp_enter);j++)
{
digitalWrite(tempde,HIGH);
cl6.println("tempdecrease is made high");
delay(500);
digitalWrite(tempde,LOW);
delay(2000);
cl6.println("tempdecrease is made low");
}
}
// digitalWrite(tempde,HIGH);
// cl6.println("tempdecrease is PRESSED");
}
*/
void tempdecrease(EthernetClient cl6)
{
digitalWrite(tempde,HIGH);
cl6.println(" decreasebutton is pressed");
delay(500);
digitalWrite(tempde,LOW);
// count2++;
// cl6.println("button is pressed :");
// cl6.println(count2);
}


