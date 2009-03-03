#!/bin/bash

java -jar saxon/saxon9.jar -o pub/File.html doc/Piston/File/File.xml xml/doc.xsl
java -jar saxon/saxon9.jar -o pub/Directory.html doc/Piston/Directory/Directory.xml xml/doc.xsl
java -jar saxon/saxon9.jar -o pub/URL.html doc/Piston/URL/URL.xml xml/doc.xsl
#java -jar saxon/saxon9.jar -o HTTPRequest.html doc/Piston/HTTP/Request/Request.xml xml/doc.xsl
