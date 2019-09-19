<?xml version="1.0" encoding="UTF-8"?>
 
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:template match="/">
        <strong><xsl:value-of select="root/sensors/bme280/d"/></strong> \n
        <!--
           <xsl:for-each select="persons/person">  -->
              <b>temperature  </b>  <xsl:value-of select="root/sensors/bme280/t"/> \n
              <b>humidity     </b>     <xsl:value-of select="root/sensors/bme280/h"/> \n
              <b>pressure     </b>     <xsl:value-of select="root/sensors/bme280/p"/> \n
          <!-- </xsl:for-each> -->
  </xsl:template>
</xsl:stylesheet>