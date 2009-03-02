#!/bin/bash

java -jar saxon/saxon9.jar -o File.html doc/Logan/File/File.xml xml/doc.xsl
java -jar saxon/saxon9.jar -o Directory.html doc/Logan/Directory/Directory.xml xml/doc.xsl
java -jar saxon/saxon9.jar -o URL.html doc/Logan/URL/URL.xml xml/doc.xsl
#java -jar saxon/saxon9.jar -o HTTPRequest.html doc/Logan/HTTP/Request/Request.xml xml/doc.xsl
