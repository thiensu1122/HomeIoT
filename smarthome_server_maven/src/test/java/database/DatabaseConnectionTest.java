package database;

import model.Android;
import model.Hub;
import model.User;
import org.junit.jupiter.api.Test;

import java.sql.Connection;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class DatabaseConnectionTest {
    DatabaseConnection databaseConnection = new DatabaseConnection();
    @Test
    void testGetConnection() {
        Connection con = databaseConnection.getConnection();
        assertNotNull(con);
    }

    @Test
    void testGetAllUser() {
        List<User> users = databaseConnection.getAllUser();
        User user = users.get(0);
        assertAll("Correct ",
                () -> assertEquals(1, user.getUser_id()),
                () -> assertEquals("bao", user.getName()),
                () -> assertEquals("bao@gmail.com", user.getEmail()),
                () -> assertEquals("active", user.getStatus())
        );
    }

    @Test
    void testGetAndroidsFromUser() {
        List<User> users = databaseConnection.getAllUser();
        User user = users.get(0);
        List<Android> androids = databaseConnection.getAndroidsfromUser(user);
        Android android = androids.get(0);
        assertAll("Correct ",
                () -> assertEquals(1, android.getUser_id()),
                () -> assertEquals(1, android.getAndroid_id()),
                () -> assertEquals("bao-android", android.getName())
        );
    }

    @Test
    void testGetHubsFromUser() {
        List<User> users = databaseConnection.getAllUser();
        List<Hub> hubs = databaseConnection.getHubfromUser(users.get(0));
        Hub hub = hubs.get(0);
        assertAll("Correct ",
                () -> assertEquals(1, hub.getUser_id()),
                () -> assertEquals(1, hub.getHub_id()),
                () -> assertEquals("bao-hub", hub.getName()),
                () -> assertEquals("LivingRoom", hub.getLocation())
        );
    }

}