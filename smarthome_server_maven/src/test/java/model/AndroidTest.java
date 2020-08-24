package model;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class AndroidTest {

    @Test
    void getUser_id() {
        Android android = new Android();
        android.setName("rocky");
        assertEquals("rocky", android.getName());
    }

    @Test
    void setUser_id() {
    }
}