package com.example.androidsmarthome.Utilities;


public enum DirectionCode {
    ERROR(-1, "Fail"),
    HUBTOSERVER(0, "Hub to server "),
    HUBTOANDROID(1, "Hub to android"),
    SERVERTOHUB(3, "Server to hub"),
    SERVERTOANDROID(4, "Server to Android"),
    ANDROIDTOHUB(5, "Android to Hub"),
    ANDROIDTOSERVER(6, "Android to server")
    ;



    private final int code;
    private final String description;

    DirectionCode(int code, String description) {
        this.code = code;
        this.description = description;
    }


    public static int getReverseDirectionCode(int code){

        //Utility.printOut("Code : " + code);
        if(code == (HUBTOSERVER.getCode())){
            return SERVERTOHUB.getCode();
        }else if (code == (ANDROIDTOSERVER.getCode())){
            return SERVERTOANDROID.getCode();
        } else {
            return ERROR.getCode();
        }
    }

    public int getCode() {
        return code;
    }

    public String getDescription() {
        return description;
    }
}
