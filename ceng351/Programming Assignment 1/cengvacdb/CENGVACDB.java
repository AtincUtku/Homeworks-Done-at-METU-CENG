package ceng.ceng351.cengvacdb;
import java.sql.*;
import java.util.ArrayList;

public class CENGVACDB implements ICENGVACDB {
    private static String user = "e2380061"; // TODO: Your userName
    private static String password = "ILRqw!3AEgY+"; //  TODO: Your password
    private static String host = "144.122.71.121"; // host name
    private static String database = "db2380061"; // TODO: Your database name
    private static int port = 8080; // port
    private static Connection connection = null;

    @Override
    public void initialize() {

        String url = "jdbc:mysql://" + host + ":" + port + "/" + database;

        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            connection =  DriverManager.getConnection(url, user, password);
        }
        catch (SQLException | ClassNotFoundException e) {
            e.printStackTrace();
        }

    }

    @Override
    public int createTables() {
        int createdTableNum = 0;

        String createUserTableSql = "CREATE TABLE User (" +
                                        "userID INT," +
                                        "userName VARCHAR(30)," +
                                        "age INT," +
                                        "address VARCHAR(150)," +
                                        "password VARCHAR(30)," +
                                        "status VARCHAR(15)," +
                                        "PRIMARY KEY (userID)" +
                                    ");";

        String createVaccineTableSql = "CREATE TABLE Vaccine (" +
                                            "code INT," +
                                            "vaccinename VARCHAR(30)," +
                                            "type VARCHAR(30)," +
                                            "PRIMARY KEY (code)" +
                                        ");";

        String createVaccinationTableSql = "CREATE TABLE Vaccination (" +
                                                "code INT," +
                                                "userID INT," +
                                                "dose INT," +
                                                "vacdate DATE," +
                                                "PRIMARY KEY (code, userID, dose)," +
                                                "FOREIGN KEY (code) REFERENCES Vaccine(code) ON DELETE CASCADE ON UPDATE CASCADE," +
                                                "FOREIGN KEY (userID) REFERENCES User(userID) ON DELETE CASCADE ON UPDATE CASCADE" +
                                            ");";
        String createAllergicSideEffectTableSql = "CREATE TABLE AllergicSideEffect (" +
                                                        "effectcode INT," +
                                                        "effectname VARCHAR(50)," +
                                                        "PRIMARY KEY (effectcode)" +
                                                    ");";

        String createSeenTableSql = "CREATE TABLE Seen (" +
                                        "effectcode INT," +
                                        "code INT," +
                                        "userID INT," +
                                        "date DATE," +
                                        "degree VARCHAR(50)," +
                                        "PRIMARY KEY (effectcode, code, userID)," +
                                        "FOREIGN KEY (effectcode) REFERENCES AllergicSideEffect(effectcode) ON DELETE CASCADE ON UPDATE CASCADE," +
                                        "FOREIGN KEY (code) REFERENCES Vaccination(code) ON DELETE CASCADE ON UPDATE CASCADE," +
                                        "FOREIGN KEY (userID) REFERENCES User(userID) ON DELETE CASCADE ON UPDATE CASCADE" +
                                    ");";
        try {
            Statement statement = this.connection.createStatement();
            statement.executeUpdate(createUserTableSql);
            createdTableNum++;
            statement.close();
        } catch (SQLException exce) {
            exce.printStackTrace();
        }

        try {
            Statement statement = this.connection.createStatement();
            statement.executeUpdate(createVaccineTableSql);
            createdTableNum++;
            statement.close();
        } catch (SQLException exce) {
            exce.printStackTrace();
        }


        try {
            Statement statement = this.connection.createStatement();
            statement.executeUpdate(createVaccinationTableSql);
            createdTableNum++;
            statement.close();
        } catch (SQLException exce) {
            exce.printStackTrace();
        }


        try {
            Statement statement = this.connection.createStatement();
            statement.executeUpdate(createAllergicSideEffectTableSql);
            createdTableNum++;
            statement.close();
        } catch (SQLException exce) {
            exce.printStackTrace();
        }


        try {
            Statement statement = this.connection.createStatement();
            statement.executeUpdate(createSeenTableSql);
            createdTableNum++;
            statement.close();
        } catch (SQLException exce) {
            exce.printStackTrace();
        }



        return createdTableNum;
    }

    @Override
    public int dropTables() {
        int droppedTableNum=0;

        String dropUserTableSql = "DROP TABLE IF EXISTS User";
        String dropVaccineTableSql = "DROP TABLE IF EXISTS Vaccine";
        String dropVaccinationTableSql = "DROP TABLE IF EXISTS Vaccination";
        String dropAllergicSideEffectTableSql = "DROP TABLE IF EXISTS AllergicSideEffect";
        String dropSeenTableSql = "DROP TABLE IF EXISTS Seen";

        try {
            Statement statement = this.connection.createStatement();

            statement.executeUpdate(dropSeenTableSql);
            droppedTableNum++;

            statement.close();
        } catch (SQLException exce) {
            exce.printStackTrace();
        }

        try {
            Statement statement = this.connection.createStatement();

            statement.executeUpdate(dropAllergicSideEffectTableSql);
            droppedTableNum++;

            statement.close();
        } catch (SQLException exce) {
            exce.printStackTrace();
        }

        try {
            Statement statement = this.connection.createStatement();

            statement.executeUpdate(dropVaccinationTableSql);
            droppedTableNum++;

            statement.close();
        } catch (SQLException exce) {
            exce.printStackTrace();
        }

        try {
            Statement statement = this.connection.createStatement();

            statement.executeUpdate(dropVaccineTableSql);
            droppedTableNum++;

            statement.close();
        } catch (SQLException exce) {
            exce.printStackTrace();
        }


        try {
            Statement statement = this.connection.createStatement();

            statement.executeUpdate(dropUserTableSql);
            droppedTableNum++;

            statement.close();
        } catch (SQLException exce) {
            exce.printStackTrace();
        }


        return droppedTableNum;
    }

    @Override
    public int insertUser(User[] users) {
        int countRow = 0;

        for (User user: users) {
            try {
                String insertion = "INSERT INTO User VALUES (?, ?, ?, ?, ?, ?);";
                PreparedStatement state = this.connection.prepareStatement(insertion);

                state.setInt(1, user.getUserID());
                state.setString(2, user.getUserName());
                state.setInt(3, user.getAge());
                state.setString(4, user.getAddress());
                state.setString(5, user.getPassword());
                state.setString(6, user.getStatus());
                state.executeUpdate();
                countRow++;

                state.close();
            } catch (SQLException exce) {
                exce.printStackTrace();
            }

        }

        return countRow;
    }

    @Override
    public int insertAllergicSideEffect(AllergicSideEffect[] sideEffects) {
        int countRow = 0;

        for (AllergicSideEffect effect: sideEffects) {
            try {
                String insertion = "INSERT INTO AllergicSideEffect VALUES (?, ?);";
                PreparedStatement state = this.connection.prepareStatement(insertion);

                state.setInt(1, effect.getEffectCode());
                state.setString(2, effect.getEffectName());
                state.executeUpdate();
                countRow++;

                state.close();

            } catch (SQLException exce) {
                exce.printStackTrace();

            }
        }


        return countRow;
    }

    @Override
    public int insertVaccine(Vaccine[] vaccines) {
        int countRow = 0;

        for (Vaccine vacc: vaccines) {
            try {
                String insertion = "INSERT INTO Vaccine VALUES (?, ?, ?);";
                PreparedStatement state = this.connection.prepareStatement(insertion);

                state.setInt(1, vacc.getCode());
                state.setString(2, vacc.getVaccineName());
                state.setString(3, vacc.getType());
                state.executeUpdate();
                countRow++;

                state.close();

            } catch (SQLException exce) {
                exce.printStackTrace();
            }
        }

        return countRow;
    }

    @Override
    public int insertVaccination(Vaccination[] vaccinations) {
        int countRow = 0;

        for (Vaccination vaccination: vaccinations) {
            try {
                String insertion = "INSERT INTO Vaccination VALUES (?, ?, ?, ?);";
                PreparedStatement state = this.connection.prepareStatement(insertion);

                state.setInt(1, vaccination.getCode());
                state.setInt(2, vaccination.getUserID());
                state.setInt(3, vaccination.getDose());
                state.setString(4, vaccination.getVacdate());
                state.executeUpdate();
                countRow++;

                state.close();


            } catch (SQLException exce) {
                exce.printStackTrace();
            }
        }

        return countRow;
    }

    @Override
    public int insertSeen(Seen[] seens) {
        int countRow = 0;

        for (Seen seen: seens) {
            try {
                String insertion = "INSERT INTO Seen VALUES (?, ?, ?, ?, ?);";
                PreparedStatement state = this.connection.prepareStatement(insertion);

                state.setInt(1, seen.getEffectcode());
                state.setInt(2, seen.getCode());
                state.setString(3, seen.getUserID());
                state.setString(4, seen.getDate());
                state.setString(5, seen.getDegree());
                state.executeUpdate();
                countRow++;

                state.close();

            } catch (SQLException exce) {
                exce.printStackTrace();
            }
        }

        return countRow;
    }

    @Override
    public Vaccine[] getVaccinesNotAppliedAnyUser() {
        ResultSet resSet;
        ArrayList<Vaccine> resList = new ArrayList<>();
        Vaccine resNode;
        Vaccine result[];

        String query = "SELECT V.code, V.vaccinename, V.type " +
                        "FROM Vaccine V " +
                        "WHERE V.code NOT IN (SELECT A.code FROM Vaccination A) " +
                        "ORDER BY V.code ASC;";

        try {
            PreparedStatement statement = this.connection.prepareStatement(query);
            resSet = statement.executeQuery();

            while (resSet.next()) {
                resNode = new Vaccine(
                        resSet.getInt("code"),
                        resSet.getString("vaccinename"),
                        resSet.getString("type")
                );
                resList.add(resNode);
            }

            statement.close();

        } catch (SQLException exce) {
            exce.printStackTrace();
        }

        result = new Vaccine[resList.size()];
        return resList.toArray(result);
    }

    @Override
    public QueryResult.UserIDuserNameAddressResult[] getVaccinatedUsersforTwoDosesByDate(String vacdate) {
        ResultSet resSet;
        ArrayList<QueryResult.UserIDuserNameAddressResult> resList = new ArrayList<>();
        QueryResult.UserIDuserNameAddressResult resNode;
        QueryResult.UserIDuserNameAddressResult result[];
        String query = "SELECT U.userID, U.userName, U.address " +
                        "FROM User U " +
                        "WHERE U.userID IN " + "(SELECT V.userID " +
                                                "FROM Vaccination V " +
                                                "WHERE V.vacdate >= ? " +
                                                "GROUP BY V.userID " +
                                                "HAVING COUNT(*) = 2) " +
                        "ORDER BY U.userID ASC;";

        try {
            PreparedStatement state = this.connection.prepareStatement(query);
            state.setString(1, vacdate);
            resSet = state.executeQuery();

            while (resSet.next()) {
                resNode = new QueryResult.UserIDuserNameAddressResult(
                        resSet.getString("userID"),
                        resSet.getString("userName"),
                        resSet.getString("address")
                );

                resList.add(resNode);
            }

            state.close();

        } catch (SQLException exce) {
            exce.printStackTrace();
        }
        result =  new QueryResult.UserIDuserNameAddressResult[resList.size()];
        return resList.toArray(result);
    }

    @Override
    public Vaccine[] getTwoRecentVaccinesDoNotContainVac() {
        ResultSet resSet;
        ArrayList<Vaccine> resList = new ArrayList<>();
        Vaccine resNode;
        Vaccine result[];
        String query = "SELECT T.code, T.vaccinename, T.type " +
                        "FROM " + "(SELECT V.code, V.vaccinename, V.type, A.vacdate " +
                                    "FROM Vaccine V, Vaccination A " +
                                    "WHERE V.code = A.code AND V.vaccinename NOT LIKE '%vac%' " +
                                    "AND A.vacdate IN (SELECT MAX(B.vacdate) " +
                                    "FROM Vaccination B, Vaccine V2 " + "WHERE B.code = V2.code AND V2.vaccinename NOT LIKE '%vac%' AND V.code = V2.code) " +
                        "ORDER BY A.vacdate DESC " + "LIMIT 2) AS T " + "ORDER BY T.code ASC;";
        try {
            PreparedStatement state = this.connection.prepareStatement(query);
            resSet = state.executeQuery();

            while (resSet.next()) {
                resNode = new Vaccine(
                        resSet.getInt("code"),
                        resSet.getString("vaccinename"),
                        resSet.getString("type")
                );

                resList.add(resNode);
            }

            state.close();

        } catch (SQLException exce) {
            exce.printStackTrace();
        }
        result = new Vaccine[resList.size()];
        return resList.toArray(result);
    }

    @Override
    public QueryResult.UserIDuserNameAddressResult[] getUsersAtHasLeastTwoDoseAtMostOneSideEffect() {
        ResultSet resSet;
        ArrayList<QueryResult.UserIDuserNameAddressResult> resList = new ArrayList<>();
        QueryResult.UserIDuserNameAddressResult resNode;
        QueryResult.UserIDuserNameAddressResult result[];

        String query = "SELECT DISTINCT U.userID, U.userName, U.address " +
                        "FROM User U, Vaccination V " +
                        "WHERE U.userID = V.userID AND V.dose > 1 AND U.userID NOT IN " +
                            "(SELECT S.userID " +
                            "FROM Seen S " +
                            "WHERE S.userID = U.userID " +
                            "GROUP BY S.userID " +
                            "HAVING COUNT(*) > 1) " +
                        "ORDER BY userID ASC;";
        try {
            PreparedStatement state = this.connection.prepareStatement(query);
            resSet = state.executeQuery();

            while (resSet.next()) {
                resNode = new QueryResult.UserIDuserNameAddressResult(
                        resSet.getString("userID"),
                        resSet.getString("userName"),
                        resSet.getString("address")
                );

                resList.add(resNode);
            }

            state.close();
        } catch (SQLException exce) {
            exce.printStackTrace();
        }

        result = new QueryResult.UserIDuserNameAddressResult[resList.size()];
        return resList.toArray(result);
    }

    @Override
    public QueryResult.UserIDuserNameAddressResult[] getVaccinatedUsersWithAllVaccinesCanCauseGivenSideEffect(String effectname) {
        ResultSet resSet;
        ArrayList<QueryResult.UserIDuserNameAddressResult> resList = new ArrayList<>();
        QueryResult.UserIDuserNameAddressResult resNode;
        QueryResult.UserIDuserNameAddressResult result[];
        String query = "SELECT DISTINCT U.userID, U.userName, U.address " +
                        "FROM User U " +
                        "WHERE NOT EXISTS " + "(SELECT S.code " +
                                                "FROM Seen S, AllergicSideEffect A " +
                                                "WHERE S.effectcode = A.effectcode AND " +
                                                "A.effectname = \"" + effectname + "\" AND NOT EXISTS " +
                                                    "(SELECT V.code " +
                                                    "FROM Vaccination V " +
                                                    "WHERE V.code = S.code AND " + "V.userID = U.userID)) " +
                        "ORDER BY U.userID ASC;";

        try {
            PreparedStatement state = this.connection.prepareStatement(query);

            resSet = state.executeQuery();

            while (resSet.next()) {
                resNode = new QueryResult.UserIDuserNameAddressResult(
                        resSet.getString("userID"),
                        resSet.getString("userName"),
                        resSet.getString("address")

                );

                resList.add(resNode);
            }

            state.close();

        } catch (SQLException exce) {
            exce.printStackTrace();
        }
        result = new QueryResult.UserIDuserNameAddressResult[resList.size()];
        return resList.toArray(result);
    }

    @Override
    public QueryResult.UserIDuserNameAddressResult[] getUsersWithAtLeastTwoDifferentVaccineTypeByGivenInterval(String startdate, String enddate) {
        ResultSet resSet;
        ArrayList<QueryResult.UserIDuserNameAddressResult> resList = new ArrayList<>();
        QueryResult.UserIDuserNameAddressResult resNode;
        QueryResult.UserIDuserNameAddressResult result[];

        String query = "SELECT DISTINCT U.userID, U.userName, U.address " +
                        "FROM User U, Vaccination V1, Vaccination V2 " +
                        "WHERE U.userID = V1.userID AND " + "V1.userID = V2.userID AND " + "V1.code <> V2.code AND " +
                                                            "V1.vacdate <= \"" + enddate + "\" AND V1.vacdate >= \"" + startdate + "\" " +
                                                            "AND V2.vacdate <= \"" + enddate + "\" AND V2.vacdate >= \"" + startdate + "\" " +
                        "ORDER BY userID ASC;";

        try {
            PreparedStatement state = this.connection.prepareStatement(query);
            resSet = state.executeQuery();

            while (resSet.next()) {
                resNode = new QueryResult.UserIDuserNameAddressResult(
                        resSet.getString("userID"),
                        resSet.getString("userName"),
                        resSet.getString("address")
                );

                resList.add(resNode);
            }

            state.close();

        } catch (SQLException exce) {
            exce.printStackTrace();
        }

        result = new QueryResult.UserIDuserNameAddressResult[resList.size()];
        return resList.toArray(result);

    }

    @Override
    public AllergicSideEffect[] getSideEffectsOfUserWhoHaveTwoDosesInLessThanTwentyDays() {
        ResultSet resSet;
        ArrayList<AllergicSideEffect> resList = new ArrayList<>();
        AllergicSideEffect resNode;
        AllergicSideEffect result[];

        String query = "SELECT E.effectcode, E.effectname " +
                        "FROM AllergicSideEffect E, Seen S, Vaccination V1, Vaccination V2 " +
                        "WHERE S.userID = V1.userID AND V1.userID = V2.userID " + "AND S.effectcode = E.effectcode AND V1.dose - V2.dose = 1 " +
                        "AND DATEDIFF(V1.vacdate, V2.vacdate) < 20 " + "ORDER BY E.effectcode ASC;";

        try {
            PreparedStatement state = this.connection.prepareStatement(query);
            resSet = state.executeQuery();

            while (resSet.next()) {
                resNode = new AllergicSideEffect(
                        resSet.getInt("effectcode"),
                        resSet.getString("effectname")
                );

                resList.add(resNode);
            }

            state.close();

        } catch (SQLException exce) {
            exce.printStackTrace();
        }

        result = new AllergicSideEffect[resList.size()];
        return resList.toArray(result);
    }

    @Override
    public double averageNumberofDosesofVaccinatedUserOverSixtyFiveYearsOld() {
        double avgDose = 0;
        ResultSet resSet;
         String query = "SELECT AVG(T.dosed) AS avgDose " + "FROM (SELECT MAX(V.dose) AS dosed, V.userID "
                                                + "FROM Vaccination V, User U " +
                                                "WHERE U.userID = V.userID AND U.age > 65 GROUP BY V.userID) AS T;";

        /*String query = "SELECT (SUM(T.dosed) / COUNT(DISTINCT T.user)) AS avgDose " +
                        "FROM (SELECT MAX(V.dose) AS dosed, " + "V.userID AS user, " +
                        "V.code FROM Vaccination V, User U " + "WHERE U.userID = V.userID AND " +
                        "U.age > 65 GROUP BY V.userID, V.code) AS T;"; */
        try {
            PreparedStatement state = this.connection.prepareStatement(query);
            resSet = state.executeQuery();

            while (resSet.next()) {
                avgDose = resSet.getDouble("avgDose");
            }

            state.close();
        } catch (SQLException exce) {
            exce.printStackTrace();
        }
        return avgDose;
    }

    @Override
    public int updateStatusToEligible(String givendate) {
        int rowNum = 0;

        String query = "UPDATE User U " +
                        "SET U.status = \"Eligible\" " +
                        "WHERE U.status = \"Not_Eligible\" " +
                            "AND (SELECT DATEDIFF(?, " + "(SELECT MAX(V.vacdate) FROM Vaccination V WHERE V.userID = U.userID))) >= 120;";

        try {
            PreparedStatement state = this.connection.prepareStatement(query);
            state.setString(1, givendate);
            rowNum = state.executeUpdate();

        } catch (SQLException exce) {
            exce.printStackTrace();
        }

        return rowNum;
    }

    @Override
    public Vaccine deleteVaccine(String vaccineName) {
        String query = "DELETE FROM Vaccine WHERE vaccinename = ?;";
        String select = "SELECT V.code, V.vaccinename, V.type FROM Vaccine V WHERE vaccinename = ?;";
        Vaccine result = null;
        ResultSet resSet;

        try {
            PreparedStatement state1 = this.connection.prepareStatement(select);
            state1.setString(1, vaccineName);
            resSet = state1.executeQuery();

            resSet.next();
            int rCode = resSet.getInt("code");
            String rName = resSet.getString("vaccinename");
            String rType = resSet.getString("type");

            result = new Vaccine(rCode, rName, rType);

            state1.close();

            try {
                PreparedStatement state2 = this.connection.prepareStatement(query);
                state2.setString(1, vaccineName);
                state2.executeQuery();
                state2.close();


            } catch (SQLException exception) {
                exception.printStackTrace();
            }

            return result;

        } catch (SQLException exce) {
            exce.printStackTrace();
        }

        return null;
    }
}
