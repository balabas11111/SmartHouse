Spring boot application which manages Abstract items
-------------------------------------------------------------
Possible implementation of abstract viewer/editor of classes which extends ItemAbstract

-----> expected functionality

list view;
batch upload by json file
entity view (partially imlemented)
entity edit (partially imlemented)
entity list as pdf report (currently no filter by accepts header)

controllers which will be able to execute CRUD operations as income data will accept children of ItemAbstract in JSON format produce result in JSON (TODO).

------------> managed classes <------------

PhoneCompany
PhoneNumber
PhoneUser

PhoneUser can have many PhoneNumber

PhoneUser is not owner of PhoneNumber
many users can have same PhoneNumbers (a.e. many people sit in the same office and use shared phone number)

PhoneNumber belongs to one PhoneCompany

Application urls
-------------------------------------------------------------

localhost:8080/PhoneCompany
localhost:8080/PhoneNumber
localhost:8080/PhoneUser

Swagger descriptor
-------------------------------------------------------------
localhost:8080/swagger-ui.html
localhost:8080/v2/api-docs

Entities Import
-------------------------------------------------------------
1. Upload resources/json/PhoneNumber.json on  localhost:8080/PhoneNumber
2. Upload resources/json/PhoneUser.json on  localhost:8080/PhoneUser




