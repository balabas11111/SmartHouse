package com.balabas.html.packager;

import javax.naming.ConfigurationException;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;

public class ConfigurationUtility {

    public static PackageConfiguration parseConfiguration(String[] args) throws ParseException, ConfigurationException{
        Options options = new Options();
        options.addOption(EspPackager.COMMON_COMP, true, "common folder with components");
        options.addOption(EspPackager.PROJECT_COMP, true, "Project folder with components");
        options.addOption(EspPackager.PROJECT_SOURCE, true, "Project own html pages");
        options.addOption(EspPackager.PROJECT_TARGET, true, "Target files");
        
        options.addOption(EspPackager.PROJECT_NAME, true, "Project name");
        options.addOption(EspPackager.BUILD_INFO, false, "Build info");
        options.addOption(EspPackager.IP, true, "Device Ip");
        
        CommandLineParser parser = new DefaultParser();
        CommandLine cmd = parser.parse( options, args);
        
        for(Option opt:cmd.getOptions()){
            System.out.println(opt.getOpt()+"="+opt.getValue());
        }
        
        return new PackageConfiguration(cmd);
    }
}
