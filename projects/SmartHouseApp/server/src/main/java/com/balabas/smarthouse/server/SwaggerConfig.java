package com.balabas.smarthouse.server;

public class SwaggerConfig {
/*
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import com.google.common.base.Predicate;
import com.google.common.base.Predicates;
import springfox.documentation.builders.ApiInfoBuilder;
import springfox.documentation.builders.PathSelectors;
import springfox.documentation.builders.RequestHandlerSelectors;
import springfox.documentation.service.ApiInfo;
import springfox.documentation.spi.DocumentationType;
import springfox.documentation.spring.web.plugins.Docket;
import springfox.documentation.swagger2.annotations.EnableSwagger2; 
 
@Configuration
@EnableSwagger2
public class SwaggerConfig {
	

	//http://localhost:8080/swagger-ui.html
    //http://localhost:8080/v2/api-docs

	
	@Value("${smarthouse.server.swg.api.name:SmartHouse server api}")
	private String apiName;
	
	@Value("${smarthouse.server.swg.api.description:SmartHouse server api description}")
	private String apiDescription;
	
	@Value("${smarthouse.server.swg.api.version:1.0}")
	private String apiVersion;
	
	@Bean
	public Docket produceApi() {
		return new Docket(DocumentationType.SWAGGER_2).apiInfo(apiInfo()).select()
				.apis(RequestHandlerSelectors.basePackage("com.balabas")).paths(paths()).build();
	}

	private ApiInfo apiInfo() {
		return new ApiInfoBuilder().title(apiName)
				.description(apiDescription).version(apiVersion)
				.build();
	}

	private Predicate<String> paths() {
	    return Predicates.not(PathSelectors.regex("/error.*"));
	}
*/
}
