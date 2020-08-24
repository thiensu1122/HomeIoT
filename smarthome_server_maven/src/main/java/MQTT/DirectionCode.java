package MQTT;

import Utility.Utility;

public enum DirectionCode {
    ERROR("-1", "Fail"),
    HUBTOSERVER("0", "Hub to server "),
    HUBTOANDROID("1", "Hub to android"),
    SERVERTOHUB("3", "Server to hub"),
    SERVERTOANDROID("4", "Server to Android"),
    ANDROIDTOHUB("5", "Android to Hub"),
    ANDROIDTOSERVER("6", "Android to server")
    ;



    private final String code;
    private final String description;

    private DirectionCode(String code, String description) {
        this.code = code;
        this.description = description;
    }

    private DirectionCode(){
        this.code = null;
        this.description = null;
    }

    public static String getReverseDirectionCode(String code){

        //Utility.printOut("Code : " + code);
        if(code.equals(HUBTOSERVER.getCode())){
            return SERVERTOHUB.getCode();
        }else if (code.equals(ANDROIDTOSERVER.getCode())){
            return SERVERTOANDROID.getCode();
        } else {
            return ERROR.getCode();
        }
    }

    public String getCode() {
        return code;
    }

    public String getDescription() {
        return description;
    }
}
