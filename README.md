# PDBMS-IoTv.1
PDBMS - Smart Farm With IoT
1. วงจรการต่อ Node กับ Sensor LCD 
ทำกำรต่อ NodeMCU เข้ำกับ Sensor LCD แบบ I2C โดย 
1.1.ขำ GND ต่อเข้ำที่ ขำ GND ของ NodeMCU 
1.2.ขำ VCC ต่อเข้ำที่ ขำ Vin(5.5v) ของ NodeMCU 
1.3.ขำ SDA ต่อเข้ำที่ ขำ D2 ของ NodeMCU 
1.4.ขำ SCL ต่อเข้ำที่ ขำ D1 ของ NodeMCU 
2.สมัคร Domain name กับเขียว SHA256 
ทำกำรสมัคร Domain name โดยสมัครที่เว็ป Namecheap จำกนั้นก็กำรตั้งค่ำ ใน Domain name ที่เรำสมัคร โดยเข้ำไปที่ Advance DNS ทำกำรเพิ่ม HOST Record 
Type HOST Value TTL 
1. A Record / @ / IP address / Auto 
2. URL Redirect Record / @ / Domain name / Unmasked 
เมื่อทำกำรสมัคร Domain name ได้แล้วทำกำรเข้ำรหัส เขียว SHA (https://) โดยทำกำรเข้ำไปที่ Server ของเรำ และทำกำรลง SHA โดยใช้ Certbot 
INSTALL 
$ sudo apt-get update 
$ sudo apt-get install software-properties-common 
$ sudo add-apt-repository ppa:certbot/certbot 
$ sudo apt-get update 
$ sudo apt-get install python-certbot-apache 
Get Started 
$ sudo certbot –apache 
$ sudo certbot --apache certonly 
เมื่อทำงำนลง Certbot จะให้ใส่ E-mail และ Domain name ของเรำ ถ้ำยังไม่ได้สมัคร Domain จะไม่สำรมำรถทำกำรเข้ำรหัส SHA ได้ 
3. Set up Firebase 
1. สมัคร Firebase และทำกำร เพิ่มโครงกำร ตั้งชื่อ โครงกำรของเรำ 
2. หลังจำกตั้งชื่อโครงกำรของเรำ แล้ว ก็จะได้หน้ำ Console ของ โครงกำรที่เรำสร้ำงไป 
3. ให้เลือกเมนู Database จำกนั้นไปที่ Tab RULES เพื่อเข้ำจัดกำร Users authorized ของกำรเข้ำถึงของมูล เบื้องต้นเรำจะอนุญำตให้คนที่เรียกเข้ำถึงข้อมูลได้หมดทั้ง Read และ Write ข้อมูลได้ โดยแก้ไข Codeแล้ว กดปุ่ม PUBLIC เพื่อบันทึกกำรเปลี่ยนแปลง 
4.กลับมำที่ Tab Data เพื่อลองสร้ำงข้อมูลกัน โดยคลิกที่เครื่องหมำย + 
5.กดไปที่เมนู Home Overview ของ Project ที่เรำสร้ำง และ คลิกเลือกที่ Add Firebase To Your web app 
6. ในที่นี้ NodeMCU จะทำส่งข้อมูลเข้ำไปที่ WebServer ก่อนและ Server ทำกำรส่งค่ำไปยัง Firebase ด้วย RESTFUL API (Method PUT) 
function write($path="",$data=[]) 
{ 
$HTTP = [ 
'http' => [ 
'method' => 'PUT', 
'header' => 'Content-type: application/json', 
'content' => json_encode($data)] 
]; 
$context = stream_context_create($HTTP); 
$contents = file_get_contents($this->url."/".$path.".json", false, $context); 
return ($contents!=false)?true:false; 
} 
4. Set up mySQL 
1. ทำกำรลง ติดตั้ง Apache2 / PHP7.0 / MySQL / phpMyadmin 
2.เข้ำที่ phpMyadmin และทำกำรสร้ำงฐำนข้อมูลแบบ utf8_unicode_ci โดยจะมีฐำนข้อมูลชื่อ temp / humid / record 
Temp = อุณหภูมิ / varchar(5) 
humid = ควำมชื้น / varchar(5) 
record = เวลำที่ทำกำรบันทึกข้อมูล / timestamp(Primary) 
3.NodeMCU ส่งข้อมูลเข้ำไปยัง WebServer 
#define web_server "http://104.199.xx.xx/" // IP web server 
Voidloop 
HTTPClient http; 
http.begin(web_server); 
http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 
http.POST("id="+String(id)+","+String(tem)+","+String(hu)); //Sent Data to Database 
String res = http.getString(); 
Serial.println(res); 
4.ทำ function บันทึกผลลงบนฐำนข้อมูล 
function insert($table,$data){ 
$con = mysqli_connect($this->host , $this->username , $this->password , $this->db_name); 
mysqli_set_charset($con,"utf8"); 
$sql = "insert into ".$table." values('".$data["id"]."','".$data["temp"]."','".$data["humid"]."','".date("Y-m-d H:i:s")."')"; 
$save = mysqli_query($con,$sql); 
$con->close(); 
return $save; 
} 
5. https:// connection fingerprint 
1. กำรส่งข้อมูลแบบ https ต้องมี fingerprint หำได้โดยกำรนำ เว็ปของเรำไปวำงที่เว็ป https://www.grc.com/fingerprints.htm แล้วจะได้ fingerprint มำ เช่น DA:F3:84:CC:D9:9A:1C:22:79:EF:C2:B5:88:7F:8A:49:F5:56:0B:3D 
2.เขียนโค้ดบน Arduino 
#define web_server " domain name " //IP web server 
#define figger_p " figgerprint " // Figger print grc.com/fingerprints.htm 
ส่วนของ void loop 
http.begin(web_server,figger_p); 
6. Sleep mode Hardware+code 
#define SECONDS_DS(seconds) ((seconds)*1000000UL) 
void setup() { 
Serial.begin(115200); 
Serial.println("Setup..."); 
} 
void loop() { 
delay(1000); 
Serial.println("Sleeping"); 
ESP.deepSleep(SECONDS_DS(5)); 
} เมื่ออัพโหลดโคดแล้ว ให้ต่อขำ D0 กับ RST เพื่อให้ทำงำนในโหมด sleep 
7. สร้างสรรค์ 8. MQTT 
MQTT คือ Broker ซึ่งเป็นส่วนกลำงที่จะนำพำข้อมูลจำกอุปกรณ์ต่ำง ๆ มำกองกันไว้ที่ Broker ซึ่งเป็นไปได้ว่ำจะมีหลำย ๆ อุปกรณ์อำจจะเป็น 10 20 ตัว หรืออำจจะ 100 ตัว เชื่อมต่อมำที่ Broker เดียวกัน 
โดยใช้ Netpie สร้ำง Device Key, Session Key โดยตัว 
Device Key จะเป็นฝั่งของทำง Arduino ส่งเข้ำไปยัง NETPIE 
Session Key จะเป็นข้อมูลส่งแบบ Java โดย NETPIE ทำกำรส่งข้อมูลไปยัง Web 
ใช้หลักกำร Firebase แต่เพียง Netpie เป็น MQTT และสำมำรถส่งไปหลำยๆที่พร้อมกันได้ 9. แสดงข้อมูล PHP 
PHP + SQL 
HTTP Request(Ajax) 
SQL Database 
Web Server 
Web Page 
Response text(json) 
SQL query 
10.แสดงข้อมูล Firebase 
Firebasse Library (Java script) Web Socket Protocol 
Firebase Realtime Database 
Web Page 
Json in event callback function 
