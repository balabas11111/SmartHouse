<#import "/spring.ftl" as spring />
<html>
<head>
    <title>${entityLabel_title}</title>
</head>
<body>
    <div>
        <h2>${entityLabel_header}</h2>
        <hr/>
        <a href="/PhoneCompany">
            <button type="submit">PhoneCompany</button>
        </a>
        <a href="/PhoneNumber">
            <button type="submit">PhoneNumber</button>
        </a>
        <a href="/PhoneUser">
            <button type="submit">PhoneUser</button>
        </a>
         <a href="/swagger-ui.html">
            <button type="submit">swg ui</button>
        </a>
         <a href="/v2/api-docs">
            <button type="submit">swg api docs</button>
        </a>
        <br/>
        <br/>
        <br/>
        
        <a href="/${entityClass}/create">
            <button type="submit">+ New</button>
        </a>
        <a href="/${entityClass}/upload">
            <button type="submit">Upload</button>
        </a>
        <br/>
         <a href="/${entityClass}/pdf">
            <button type="submit">PDF report</button>
        </a>
        <br/>
        <br/><br/>
        <div>
            <div>
                <table>
                    <tr>
                    <!-- TODO: add header template by entityClassName-->
                        <th>Id</th>
                        <th>Name</th>
                    </tr>
                    <#if (entities?? && entities?size != 0)>
                        <#list entities as entity>
                            <tr>
                            <!-- TODO: add entity template by entityClassName-->
                                <td>${entity.id}</td>
                                <td>${entity.name}</td>
                                <td>
                                    <a href="/${entityClass}/${entity.id}">Edit </a>
                                </td>
                                <td>
    	                            <a href="/${entityClass}/${entity.id}/delete">
									    <button type="submit">Delete</button>
									</a>
                                </td>
                            </tr>
                        </#list>
                    <#else>
                        No entities in library
                    </#if>
                </table>
            </div>
        </div>
    </div>
</body>
</html>