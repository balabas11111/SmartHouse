<?xml version="1.0" encoding="UTF-8"?>
 
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:template match="/">
        <strong><xsl:value-of select="root/bme280/d"/></strong> <br></br> <br></br>
        <!--
           <xsl:for-each select="persons/person">  -->
              <b>temperature  </b>  <xsl:value-of select="root/bme280/t"/> <br></br>
              <b>humidity     </b>     <xsl:value-of select="root/bme280/h"/> <br></br>
              <b>pressure     </b>     <xsl:value-of select="root/bme280/p"/> <br></br>
          <!-- </xsl:for-each> -->
  </xsl:template>
</xsl:stylesheet>