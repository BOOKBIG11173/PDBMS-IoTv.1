# PDBMS-IoTv.1
PDBMS - Smart Farm With IoT
![esp8266](https://user-images.githubusercontent.com/30660759/29000870-c401ea34-7aa3-11e7-87f8-8eefd82f0471.png)

#####รายละเอียดรายต่อวงจรและใช้งานเบื้องต้น#####<br />
<br />
### 1.วงจรการต่อ NodeMCU กับ Sensor LCD และ DHT22<br />
ทำการต่อ NodeMCU เข้ากับ DHT22 แบบ โดย<br />
1.ขา1-VCC     => ขา Vin(3.3v) ของ NodeMCU<br />
2.ขา2-DIGTAL  => ขา D3 ของ NodeMCU<br />
3.ขา3-NULL    => NULL ของ NodeMCU<br />
4.ขา4-GND     => ขา ของ NodeMCU <br />
ทำการต่อ NodeMCU เข้ากับ Sensor LCD แบบ I2C โดย <br />
1.ขา1-GND => ขา GND ของ NodeMCU <br /> 
2.ขา2-VCC => ขา Vin(5.5v) ของ NodeMCU <br />
3.ขา3-SDA => ขา D2 ของ NodeMCU <br />
4.ขา4-SCL => ขา D1 ของ NodeMCU<br />
### 2.สมัคร Domain Name กับเขียว SHA256<br />
ทำการสมัคร Domain name โดยสมัครที่เว็ป Namecheap จำกนั้นก็ทำการตั้งค่าใน Domain name ที่เราสมัคร โดยเข้าไปที่ Advance DNS ทำการเพิ่ม HOST Record 
Type HOST Value TTL<br />
    1. A Record / @ / IP address / Auto<br />
    2. URL Redirect Record / @ / Domain name / Unmasked<br />
เมื่อทำการสมัคร Domain name ได้แล้วทำการเข้ารหัสเขียว SHA (https://) โดยทำการเข้าไปที่ Server ของเรา และทำการลง SHA โดยใช้ Certbot<br />
INSTALL<br /> 
$ sudo apt-get update<br /> 
$ sudo apt-get install software-properties-common <br />
$ sudo add-apt-repository ppa:certbot/certbot<br /> 
$ sudo apt-get update <br />
$ sudo apt-get install python-certbot-apache <br />
Get Started <br />
$ sudo certbot –apache <br />
$ sudo certbot --apache certonly <br />
เมื่อทำการลง Certbot จะให้ใส่ E-mail และ Domain name ของเรา ถ้ายังไม่ได้สมัคร Domain จะไม่สารมารถทำการเข้ารหัส SHA ได้ <br />
### 3.Set up Firebase
1.สมัคร Firebase และทำการ เพิ่มโครงการ ตั้งชื่อโครงการของเรา<br />
2.หลังจากตั้งชื่อโครงการของเราแล้ว จะได้หน้า Console ของ โครงการที่เราสร้างไป<br />
3.ให้เลือกเมนู Database จากนั้นไปที่ Tab RULES เพื่อเข้าจัดการ Users authorized ของการเข้าถึงข้อมูล เบื้องต้นเราจะอนุญาตให้คนที่เรียกเข้าถึงข้อมูลได้หมดทั้ง Read และ Write ข้อมูลได้ โดยแก้ไข Code แล้ว กดปุ่ม PUBLIC เพื่อบันทึกกำรเปลี่ยนแปลง<br /> 
![2](https://user-images.githubusercontent.com/30660759/29001177-c56b9b06-7aab-11e7-87d9-cada0bb93ef2.png)<br />
4.กลับมาที่ Tab Data เพื่อลองสร้างข้อมูล โดยคลิกที่เครื่องหมาย + <br /> 
5.กดไปที่เมนู Home Overview ของ Project ที่เราสร้าง และ คลิกเลือกที่ Add Firebase To Your web app<br /> 
6.ในที่นี้ NodeMCU จะทำส่งข้อมูลเข้าไปที่ WebServer ก่อนและ Server ทำการส่งค่าไปยัง Firebase ด้วย RESTFUL API (Method PUT)<br /> 
function write($path="",$data=[])<br /> 
{ <br />
$HTTP = [ <br />
'http' => [ <br />
'method' => 'PUT',<br /> 
'header' => 'Content-type: application/json', <br />
'content' => json_encode($data)] <br />
]; <br />
$context = stream_context_create($HTTP); <br />
$contents = file_get_contents($this->url."/".$path.".json", false, $context); <br />
return ($contents!=false)?true:false; <br />
} <br />
### 4.Set up mySQL<br />
1. ทำกำรลง ติดตั้ง Apache2 / PHP7.0 / MySQL / phpMyadmin<br /> 
2.เข้ำที่ phpMyadmin และทำกำรสร้ำงฐำนข้อมูลแบบ utf8_unicode_ci โดยจะมีฐำนข้อมูลชื่อ temp / humid / record <br />
Temp = อุณหภูมิ / varchar(5) <br />
humid = ควำมชื้น / varchar(5) <br />
record = เวลำที่ทำกำรบันทึกข้อมูล / timestamp(Primary)<br /> 
3.NodeMCU ส่งข้อมูลเข้ำไปยัง WebServer <br />
#define web_server "http://104.199.xx.xx/" // IP web server <br />
Voidloop <br />
HTTPClient http; <br /><br />
http.begin(web_server);<br />
http.addHeader("Content-Type", "application/x-www-form-urlencoded"); <br /><br />
http.POST("id="+String(id)+","+String(tem)+","+String(hu)); //Sent Data to Database <br />
String res = http.getString(); <br />
Serial.println(res);<br /> 
4.ทำ function บันทึกผลลงบนฐำนข้อมูล <br />
function insert($table,$data){ <br />
$con = mysqli_connect($this->host , $this->username , $this->password , $this->db_name);<br /> 
mysqli_set_charset($con,"utf8"); <br />
$sql = "insert into ".$table." values('".$data["id"]."','".$data["temp"]."','".$data["humid"]."','".date("Y-m-d H:i:s")."')"; <br />
$save = mysqli_query($con,$sql); <br /><br />
$con->close(); <br />
return $save; <br />
} <br />
### 5.https:// connection fingerprint <br />
1. กำรส่งข้อมูลแบบ https ต้องมี fingerprint หำได้โดยกำรนำ เว็ปของเรำไปวำงที่เว็ป https://www.grc.com/fingerprints.htm แล้วจะได้ fingerprint มำ เช่น DA:F3:84:CC:D9:9A:1C:22:79:EF:C2:B5:88:7F:8A:49:F5:56:0B:3D <br />
2.เขียนโค้ดบน Arduino <br />
#define web_server " domain name " //IP web server <br /> 
#define figger_p " figgerprint " // Figger print grc.com/fingerprints.htm <br />
ส่วนของ void loop <br />
http.begin(web_server,figger_p); <br />
6. Sleep mode Hardware+code <br />
#define SECONDS_DS(seconds) ((seconds)*1000000UL) <br />
void setup() { <br />
Serial.begin(115200); <br />
Serial.println("Setup..."); <br />
} <br />
void loop() { <br />
delay(1000); <br />
Serial.println("Sleeping"); <br />
ESP.deepSleep(SECONDS_DS(5)); <br />
} เมื่ออัพโหลดโคดแล้ว ให้ต่อขำ D0 กับ RST เพื่อให้ทำงำนในโหมด sleep <br />
### 7.สร้างสรรค์ 
### 8.MQTT 
MQTT คือ Broker ซึ่งเป็นส่วนกลำงที่จะนำพำข้อมูลจำกอุปกรณ์ต่ำง ๆ มำกองกันไว้ที่ Broker ซึ่งเป็นไปได้ว่ำจะมีหลำย ๆ อุปกรณ์อำจจะเป็น 10 20 ตัว หรืออำจจะ 100 ตัว เชื่อมต่อมำที่ Broker เดียวกัน โดยใช้ Netpie สร้ำง Device Key, Session Key โดยตัว <br />
Device Key จะเป็นฝั่งของทำง Arduino ส่งเข้ำไปยัง NETPIE <br /><br />
Session Key จะเป็นข้อมูลส่งแบบ Java โดย NETPIE ทำกำรส่งข้อมูลไปยัง Web <br />
ใช้หลักกำร Firebase แต่เพียง Netpie เป็น MQTT และสำมำรถส่งไปหลำยๆที่พร้อมกันได้ <br />
### 9.แสดงข้อมูล PHP 
### 10.แสดงข้อมูล Firebase 

