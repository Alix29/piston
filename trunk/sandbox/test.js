//var f = new Piston.File("/mnt/deck/DevLab/piston/sandbox/tmp/test_utf8.txt", { "encoding" : "UTF-8" });
//f.readAll();

var utfString = "聚焦 拉贝日记震撼柏林";
var asciiString = "abc\t123\nxyz";

//var f = new Piston.File({"path":true});
//f.isFile();

Piston.Directory.isDirectory("/tmp");


//Piston.File.isFile("/dev/null");

//f.exists();
//Piston.File.exists("");
//var f = new Piston.File("/mnt/deck/DevLab/piston/sandbox/tmp/test_ascii.txt");
//f.readAll();

//Piston.File.readAll("/mnt/deck/DevLab/piston/sandbox/tmp/test_utf8.txt");

//var f = new Piston.File("/mnt/deck/DevLab/piston/sandbox/tmp/test_ascii.txt");

//Piston.File.size("/mnt/deck/DevLab/piston/sandbox/tmp/test_asciiz.txt");

//Piston.File.readAll("/mnt/deck/DevLab/piston/sandbox/tmp/test_ascii.txt");
//Piston.File.readAll("/mnt/deck/DevLab/piston/sandbox/tmp/test_utf8.txt");

//var f = new Piston.File("/mnt/deck/DevLab/piston/sandbox/tmp/test_asciiz.txt");
//f.size();


//var u = new Piston.URL("http://google.com/path/index.html?level=47&apple=red");
//u.unsetQueryParameter("level");

//u.toString();

//"聚焦 拉贝日记震撼柏林"


//var l8 = Piston8;
//var result = "";
//var f = new l8.File("/tmp/myfile.txt");
//if(f.exists())
//{
//	result = f.size();
//}
//delete f;

//result

//var f = new l8.File("/tmp");
//f.isDirectory()


//l8.File.exists("/tmp/myfile.txt");
//l8.File.isDirectory("/tmp");

//File.exists("/tmp/myfile.txt");



// Synchronous
/*
var resultData = Piston.HTTP.get(url);
Piston.HTTP.getToFile(url, File)
Piston.HTTP.put(url, data);
Piston.HTTP.put(url, File);
Piston.HTTP.post(url, { "level" : "47" });

var h = new HTTP(url);
h.method = "GET";
var resultData = h.get();
h.getToFile(File)*/


// Asynchronous
/*
var h = new HTTP(url);
h.method ="GET";
h.username = "username";
h.password = "password";
h.destinationFile = File;
h.subscribe("header", function(header, arguments) { }, arguments);
h.subscribe("headers", function(headers, arguments) { }, arguments);
h.subscribe("data", function(data, arguments) { }, arguments);
h.subscribe("complete", function(arguments) { }, arguments);
h.perform();

*/

/*
var result = "no result";

var f = new Piston.File("/tmp/myfile.txt");
result = f.size();

delete f;*/

//result = File.size("/tmp/myfile.txt");

/*
var url = new Piston.URL("http://sembiance:testing@abc.com:7777/some/path/to?verbose=true&sex=1#anchor47");

result + "\n" +
"protocol: " + url.protocol + "\n" +
"username: " + url.username + "\n" +
"password: " + url.password + "\n" +
"host: " + url.host + "\n" +
"port: " + url.port + "\n" +
"path: " + url.path + "\n" +
"query: " + url.query + "\n" +
"anchor: " + url.anchor + "\n"
*/