<#import "/spring.ftl" as spring />
<!DOCTYPE html>
<html lang="en">
<head>
    <title>${entityLabel_title}</title>
</head>
<body>
    <div>
        <h2>${entityLabel_header}</h2>
        <div>
            <div>
                <@spring.bind "entity"/>
                <form action="${entityClass_action}" method="post">
                    <div>
                    	<!-- TODO: add entity template by entityClassName-->
                        <div>
                             Id: ${entity.id}
                        </div>
                        <div>
                            Name:
                            <@spring.formInput "entity.name"/>
                            <@spring.showErrors "<br>"/>
                        </div>
                        <div>
                        	PhoneCompany:
                        	<@spring.formInput "entity.company.name"/>
  							<@spring.showErrors "<br>"/>
                        </div>
                    </div>
                    <div>
                        <div>
	                        <a href="/${entityClass}">
							    <button type="submit">Cancel</button>
							</a>
                            <input type="submit" value="Save">
                        </div>
                    </div>
                </form:form>
            </div>
        </div>
    </div>
    </body>
</html>