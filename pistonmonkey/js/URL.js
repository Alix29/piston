/*
 * URL.js - URL class
 * Author - robert@cosmicrealms.com
 * Thanks to http://gunblad3.blogspot.com/2008/05/uri-url-parsing.html for the regex
 */

Piston.URL = function(arg1, arg2)
{
	var self = this;
	
	self.protocol = null;
	self.username = null;
	self.password = null;
	self.host = null;
	self.port = null
	self.path = null;
	self.query = null;
	self.anchor = null;
	
	var parameterMap = null;
	
	if(typeof arg1=="string")
	{
		var urlRegex = /^((\w+):\/\/\/?)?((\w+):?(\w+)?@)?([^\/\?:]+):?(\d+)?(\/?[^\?#;\|]+)?([;\|])?([^\?#]+)?\??([^#]+)?#?(\w*)/;
		var results = urlRegex.exec(arg1);

		if(typeof results[2]!="undefined" && results[2].length>0)
			self.protocol = results[2];
			
		if(typeof results[4]!="undefined" && results[4].length>0 && typeof results[5]!="undefined" && results[5].length>0)
		{
			self.username = results[4];
			self.password = results[5];
		}
			
		if(typeof results[6]!="undefined" && results[6].length>0)
			self.host = results[6];
			
		if(typeof results[7]!="undefined" && results[7].length>0)
			self.port = parseInt(results[7]);
			
		if(typeof results[8]!="undefined" && results[8].length>0)
			self.path = results[8];
			
		if(typeof results[11]!="undefined" && results[11].length>0)
		{
			var queryParts = results[11].split("&");
			self.query = {};
			for(var i=0;i<queryParts.length;i++)
			{
				var subParts = queryParts[i].split("=");
				if(subParts.length>1)
					self.query[subParts[0]] = subParts[1];
				else if(subParts.length>0)
					self.query[subParts[0]] = "";
			}
		}
		
		if(typeof results[12]!="undefined" && results[12].length>0)
			self.anchor = results[12];
			
		if(typeof arg2=="object")
			parameterMap = arg2;
	}
	else if(typeof arg1=="object")
	{
		parameterMap = arg1;
	}
	
	if(parameterMap!=null)
	{
		for(var key in parameterMap)
		{
			if(!parameterMap.hasOwnProperty(key))
				continue;
			
			self[key] = parameterMap[key];
		}
	}
	
	self.toString = function()
	{
		var result = "";
		
		if(self.protocol!=null)
			result += self.protocol + "://";
		
		if(self.username!=null && self.password!=null)
			result += self.username + ":" + self.password + "@";
			
		if(self.host!=null)
			result += self.host;
		
		if(self.port!=null)
			result += ":" + self.port;
		
		if(self.path!=null)
			result += self.path;
		
		if(self.query!=null)
		{
			var first = true;
			for(var key in self.query)
			{
				result += first ? "?" : "&"
				first = false;
				
				result += key + "=" + self.query[key];
			}
		}
		
		if(self.anchor!=null)
			result += "#" + self.anchor;
			
		return result;
	};
	
	self.setQueryParameter = function(key, value)
	{
		if(self.query==null)
			self.query = {};
			
		self.query[key] = value;
	};
	
	self.getQueryParameter = function(key)
	{
		if(self.query==null || !self.query.hasOwnProperty(key))
			return null;
		
		return self.query[key];
	};
	
	self.unsetQueryParameter = function(key)
	{
		delete self.query[key];
	};
};
