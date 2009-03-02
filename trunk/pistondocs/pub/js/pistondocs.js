var pistondocs =
{
	init : function()
	{
		YAHOO.util.Dom.getElementsByClassName("expandLongDescription", "span", document.body, function(obj) { YAHOO.util.Event.addListener(obj, "click", docs.expandLongDescriptionClicked, obj); });
		YAHOO.util.Dom.getElementsByClassName("expand", "td", document.body, function(obj) { YAHOO.util.Event.addListener(obj, "click", docs.expandClicked, obj); });
		
		sh_highlightDocument();
	},
	
	expandLongDescriptionClicked : function(evt, arg)
	{
		arg.style.display = "none";
		
		YAHOO.util.Dom.getElementsByClassName("longDescription", "div", document.body, function(obj) { obj.style.display = "block"; });
	},
	
	expandClicked : function(evt, arg)
	{
		for(var bottom=arg.parentNode;!bottom.className || bottom.className.indexOf("bottom")==-1;bottom=bottom.nextSibling)
		;
				
		var longDesc =  YAHOO.util.Dom.getElementsByClassName("long", "div", bottom);
		
		if(arg.className=="expand")
		{
			arg.className = "expanded";
			longDesc[0].style.display = "block";
		}
		else
		{
			arg.className = "expand";
			longDesc[0].style.display = "none";
		}
	}
};

YAHOO.util.Event.onDOMReady(pistondocs.init);