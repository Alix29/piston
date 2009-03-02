<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:fn="http://www.w3.org/2005/xpath-functions"
	xmlns:f="http://piston.googlecode.com/svn/trunk/ns/common"
	version="2.0">
	<xsl:function name="f:reverse">
		<xsl:param name="theString" />
		<xsl:variable name="thisLength" select="string-length($theString)" />
		<xsl:choose>
			<xsl:when test="$thisLength = 1">
				<xsl:value-of select="$theString" />
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="concat(substring($theString, $thisLength, 1), f:reverse(substring($theString,  1, $thisLength -1)))" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:function>
	<xsl:function name="f:ltrim">
		<xsl:param name="s" />
		<xsl:choose>
			<xsl:when test="substring($s, 1, 1) = ''">
				<xsl:value-of select="$s" />
			</xsl:when>
			<xsl:when test="normalize-space(substring($s, 1, 1)) = ''">
				<xsl:value-of select="f:ltrim(substring($s, 2))"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$s" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:function>
	<xsl:function name="f:rtrim">
		<xsl:param name="s" />
		<xsl:choose>
			<xsl:when test="substring($s, 1, 1) = ''">
				<xsl:value-of select="$s" />
			</xsl:when>
			<xsl:when test="normalize-space(substring($s, string-length($s))) = ''">
				<xsl:value-of select="f:rtrim(substring($s, 1, string-length($s) - 1))"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$s" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:function>
	<xsl:function name="f:trim">
		<xsl:param name="s" />
		<xsl:value-of select="f:rtrim(f:ltrim($s))"/>
	</xsl:function>
	<xsl:function name="f:num">
		<xsl:param name="n" />
		<xsl:choose>
			<xsl:when test="string(number($n))='NaN'">
				<xsl:text>0</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="number($n)"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:function>
</xsl:stylesheet>