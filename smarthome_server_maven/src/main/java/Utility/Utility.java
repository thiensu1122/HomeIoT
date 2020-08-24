package Utility;

import MQTT.MessageCode;

public class Utility {
//    public static String getChannelHubPrefix(){
//        return "Smarthome-hub-";
//    }
//    public static String getChannleAndroidPrefix() {return "Smarthome_android-";}
    public static String getChannelPrefix() {
        return "Smarthome-";
    }


    public static void printOut(String text){
        System.out.println(text);
    }


    public static boolean isSensorMessage(int code) {
        return (50 <= code && code <100);
    }

    public static boolean isDeviceMessage(int code) {
        return (100<= code && code <200);
    }

    public static boolean isConfirmMessage(int code) {
        return code == MessageCode.CONFIRM.getCode();
    }

}
