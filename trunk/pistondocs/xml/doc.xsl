<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:d="http://piston.googlecode.com/svn/trunk/ns/doc"
    xmlns:f="http://piston.googlecode.com/svn/trunk/ns/common"
    version="2.0">
    <xsl:output indent="yes" omit-xml-declaration="yes" method="html"/>
    <xsl:include href="common.xsl"/>
    <xsl:template match="/d:doc">
        <html>
            <head>
                <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
                <title><xsl:value-of select="name"/></title>
                <link rel="stylesheet" type="text/css" href="yui/reset.css"/>
                <link rel="stylesheet" type="text/css" href="yui/fonts.css"/>
                <link rel="stylesheet" type="text/css" href="css/doc.css"/>
                <link rel="stylesheet" type="text/css" href="css/sh_style.css"/>
                            
                <script type="text/javascript" src="yui/yahoo.js"></script>
                <script type="text/javascript" src="yui/event.js"></script>
                <script type="text/javascript" src="yui/dom.js"></script>
                <script type="text/javascript" src="js/pistondocs.js"></script>
                <script type="text/javascript" src="js/sh_main.js"></script>
                <script type="text/javascript" src="js/sh_javascript.js"></script>
            </head>
            <body>
                <div class="top-section-links">
                    <xsl:if test="count(constructors/constructor) > 0">
                        <a class="top-section-link" href="#Constructors">Constructors</a>
                    </xsl:if>
                    <xsl:if test="count(properties/property) > 0">
                        <a class="top-section-link" href="#Properties">Properties</a>
                    </xsl:if>
                    <xsl:if test="count(methods/method) > 0">
                        <a class="top-section-link" href="#Methods">Methods</a>
                    </xsl:if>
                    <xsl:if test="count(events/event) > 0">
                        <a class="top-section-link" href="#Events">Events</a>
                    </xsl:if>
                </div>
                <h1><xsl:value-of select="name"/></h1>
                <div class="shortDescription">
                    <xsl:copy-of select="shortDescription/child::text() | shortDescription/child::node()"/>
                </div>
                <xsl:if test="count(longDescription)>0 or count(example)>0">
                    <span class="expandLongDescription">
                        (read full description)
                    </span>
                    <div class="longDescription">
                        <xsl:copy-of select="longDescription/child::text() | longDescription/child::node()"/>
                        <xsl:for-each select="example">
                            <div class="example">Example</div>
                            <pre class="sh_javascript"><xsl:value-of select="f:trim(.)"/></pre>
                        </xsl:for-each>
                    </div>
                </xsl:if>
                
                <xsl:if test="count(constructors/constructor) > 0">
                    <a name="Constructors"/>
                    <h2 class="constructors">Constructors</h2>
                    <table class="section constructor">
                        <tbody>
                            <xsl:for-each select="constructors/constructor">
                                <xsl:for-each select="arguments">
                                    <tr>
                                        <xsl:attribute name="class">
                                            <xsl:choose>
                                                <xsl:when test="position() = 1"><xsl:text>top</xsl:text></xsl:when>
                                                <xsl:otherwise>
                                                    <xsl:text>middle</xsl:text>
                                                </xsl:otherwise>
                                            </xsl:choose>
                                        </xsl:attribute>
                                        <xsl:if test="position() = 1">
                                            <td>
                                                <xsl:attribute name="class"><xsl:text>expand</xsl:text></xsl:attribute>
                                                <xsl:attribute name="rowspan">
                                                    <xsl:value-of select="count(../arguments)+1"/>
                                                </xsl:attribute>
                                            </td>
                                        </xsl:if>
                                        <td class="method">
                                            <span class="name">
                                                <xsl:value-of select="../../../name"/>
                                            </span>
                                            <xsl:text>(</xsl:text>
                                            <xsl:for-each select="argument">
                                                <xsl:if test="@optional = 'true'">
                                                    <xsl:text disable-output-escaping="yes">&lt;span class="optional"&gt;</xsl:text>
                                                </xsl:if>
                                                
                                                <xsl:value-of select="type"/>
                                                <xsl:text> </xsl:text>
                                                <xsl:value-of select="name"/>
                                                
                                                <xsl:if test="@optional = 'true'">
                                                    <xsl:text disable-output-escaping="yes">?&lt;/span&gt;</xsl:text>
                                                </xsl:if>
                                                
                                                <xsl:if test="position() != last()"><xsl:text>, </xsl:text></xsl:if>
                                            </xsl:for-each>
                                            <xsl:text>)</xsl:text>
                                        </td>
                                    </tr>
                                </xsl:for-each>
                                <tr class="bottom">
                                    <td class="description">
                                        <div class="short"><xsl:copy-of select="description/child::text() | description/child::node()"/></div>
                                        <div class="long">
                                            <xsl:for-each select="arguments">
                                                <div class="arguments">Arguments</div>
                                                <ul>
                                                    <xsl:if test="count(argument) = 0">
                                                        <li>None</li>
                                                    </xsl:if>
                                                    <xsl:for-each select="argument">
                                                        <li>
                                                            <xsl:if test="@optional = 'true'">
                                                                <xsl:text disable-output-escaping="yes">&lt;span class="optional"&gt;</xsl:text>
                                                            </xsl:if>
                                                            
                                                            <xsl:value-of select="type"/>
                                                            <xsl:text> </xsl:text>
                                                            <xsl:value-of select="name"/>
                                                            
                                                            <xsl:if test="@optional = 'true'">
                                                                <xsl:text disable-output-escaping="yes"> (optional)&lt;/span&gt;</xsl:text>
                                                            </xsl:if>
                                                            
                                                            <div>
                                                                <xsl:copy-of select="description/child::text() | description/child::node()"/>
                                                            </div>
                                                        </li>
                                                    </xsl:for-each>
                                                </ul>
                                            </xsl:for-each>
                                            <xsl:for-each select="example">
                                                <div class="example">Example</div>
                                                <pre class="sh_javascript"><xsl:value-of select="f:trim(.)"/></pre>
                                            </xsl:for-each>
                                        </div>
                                    </td>
                                </tr>
                            </xsl:for-each>                        
                        </tbody>
                    </table>
                </xsl:if>
                
                <xsl:if test="count(properties/property) > 0">
                    <a name="Properties"/>
                    <h2 class="properties">Properties</h2>
                    <table class="section property">
                        <tbody>
                            <xsl:for-each select="properties/property">
                                <tr class="bottom">
                                    <td class="expand"></td>
                                    <td class="type">
                                        <xsl:value-of select="type"/>
                                    </td>
                                    <td class="name">
                                        <xsl:value-of select="name"/>
                                    </td>
                                    <td class="description">
                                        <xsl:copy-of select="description/child::text() | description/child::node()"/>
                                        <div class="long">
                                            <xsl:if test="count(validValue) > 0">
                                                <div class="validValues">
                                                    Valid Values
                                                </div>
                                                <ul>
                                                    <xsl:for-each select="validValue">
                                                        <li>
                                                            <xsl:value-of select="."/>
                                                            <xsl:text> </xsl:text>
                                                            <xsl:if test="@default = 'true'">
                                                                <span class="default">(default)</span>
                                                            </xsl:if>
                                                        </li>
                                                    </xsl:for-each>
                                                </ul>
                                            </xsl:if>
                                        </div>
                                    </td>
                                    <xsl:choose>
                                        <xsl:when test="@readonly = 'true'">
                                            <td class="readOnly"><xsl:text>read-only</xsl:text></td>
                                        </xsl:when>
                                        <xsl:otherwise>
                                            <td class="readWrite"><xsl:text>read+write</xsl:text></td>
                                        </xsl:otherwise>
                                    </xsl:choose>
                                </tr>
                            </xsl:for-each>
                        </tbody>
                    </table>
                </xsl:if>
                
                <xsl:if test="count(methods/method) > 0">
                    <a name="Methods"/>
                    <h2 class="methods">Methods</h2>
                    <table class="section method">
                        <tbody>
                            <xsl:for-each select="methods/method">
                                <xsl:for-each select="arguments">
                                    <tr>
                                        <xsl:attribute name="class">
                                            <xsl:choose>
                                                <xsl:when test="position() = 1"><xsl:text>top</xsl:text></xsl:when>
                                                <xsl:otherwise>
                                                    <xsl:text>middle</xsl:text>
                                                </xsl:otherwise>
                                            </xsl:choose>
                                        </xsl:attribute>
                                        <xsl:if test="position() = 1">
                                            <td>
                                                <xsl:attribute name="class"><xsl:text>expand</xsl:text></xsl:attribute>
                                                <xsl:attribute name="rowspan">
                                                    <xsl:value-of select="count(../arguments)+1"/>
                                                </xsl:attribute>
                                            </td>
                                        </xsl:if>
                                        <td class="static">
                                            <xsl:if test="../@static = 'true' or @static = 'true'"><xsl:text>static</xsl:text></xsl:if>
                                        </td>
                                        <td class="return">
                                            <xsl:if test="count(../returns) = 0">
                                                <xsl:text>void</xsl:text>
                                            </xsl:if>
                                            <xsl:value-of select="../returns/type"/>
                                        </td>
                                        <td class="method">
                                            <span class="name">
                                                <xsl:value-of select="../name"/>
                                            </span>
                                            <xsl:text>(</xsl:text>
                                            <xsl:for-each select="argument">
                                                <xsl:if test="@optional = 'true'">
                                                    <xsl:text disable-output-escaping="yes">&lt;span class="optional"&gt;</xsl:text>
                                                </xsl:if>
                                                
                                                <xsl:value-of select="type"/>
                                                <xsl:text> </xsl:text>
                                                <xsl:value-of select="name"/>
                                            
                                                <xsl:if test="@optional = 'true'">
                                                    <xsl:text disable-output-escaping="yes">?&lt;/span&gt;</xsl:text>
                                                </xsl:if>
                                                
                                                <xsl:if test="position() != last()"><xsl:text>, </xsl:text></xsl:if>
                                            </xsl:for-each>
                                            <xsl:text>)</xsl:text>
                                        </td>
                                    </tr>
                                </xsl:for-each>
                                <tr class="bottom">
                                    <td colspan="2"></td>
                                    <td class="description">
                                        <div class="short"><xsl:copy-of select="description/child::text() | description/child::node()"/></div>
                                        <div class="long">
                                            <xsl:for-each select="arguments">
                                                <div class="arguments">
                                                    <xsl:text>Arguments</xsl:text>
                                                    <xsl:if test="@static = 'true'">
                                                        <span class="static"> static</span>
                                                    </xsl:if>
                                                </div>
                                                <ul>
                                                    <xsl:if test="count(argument) = 0">
                                                        <li>None</li>
                                                    </xsl:if>
                                                    <xsl:for-each select="argument">
                                                        <li>
                                                            <xsl:if test="@optional = 'true'">
                                                                <xsl:text disable-output-escaping="yes">&lt;span class="optional"&gt;</xsl:text>
                                                            </xsl:if>
                                                            
                                                            <xsl:value-of select="type"/>
                                                            <xsl:text> </xsl:text>
                                                            <xsl:value-of select="name"/>
                                                            
                                                            <xsl:if test="@optional = 'true'">
                                                                <xsl:text disable-output-escaping="yes"> (optional)&lt;/span&gt;</xsl:text>
                                                            </xsl:if>
                                                            
                                                            <div>
                                                                <xsl:copy-of select="description/child::text() | description/child::node()"/>
                                                            </div>
                                                        </li>
                                                    </xsl:for-each>
                                                </ul>
                                            </xsl:for-each>
                                            <div class="returns">Returns</div>
                                            <ul>
                                                <li>
                                                    <xsl:if test="count(returns) = 0">
                                                        <xsl:text>void</xsl:text>
                                                    </xsl:if>
                                                    <xsl:value-of select="returns/type"/>
                                                    <div>
                                                        <xsl:copy-of select="returns/description/child::text() | returns/description/child::node()"/>
                                                    </div>
                                                </li>
                                            </ul>
                                            <xsl:for-each select="example">
                                                <div class="example">
                                                    <xsl:text>Example</xsl:text>
                                                    <xsl:if test="@static = 'true'">
                                                        <span class="static"> static</span>
                                                    </xsl:if> 
                                                </div>
                                                <pre class="sh_javascript"><xsl:value-of select="f:trim(.)"/></pre>
                                            </xsl:for-each>
                                        </div>
                                    </td>
                                </tr>
                            </xsl:for-each>                        
                        </tbody>
                    </table>
                </xsl:if>
                
                <xsl:if test="count(events/event) > 0">
                    <a name="Events"/> 
                    <h2 class="events">Events</h2>
                </xsl:if>
            </body>
        </html>
    </xsl:template>
</xsl:stylesheet>