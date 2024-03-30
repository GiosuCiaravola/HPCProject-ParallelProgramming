/*  
*   Ciaravola Giosuè, 0622702177, g.ciaravola3@studenti.unisa.it
*   Attended course: AH
*   Prof. D'Aniello Giuseppe, gidaniello@unisa.it
*   
*   Exercise 2 – Spark
*
*   Find the team (or teams in case of a tie) that has won the most matches 
*   with the fewest number of players on the field at the end of the game.
*/

package worldcup;

import java.util.*;
import org.apache.spark.SparkConf;
import org.apache.spark.api.java.*;
import scala.Tuple2;

public class SparkDriver {

    public static void main(String[] args) {
        
        String inputPath = args[0]; 
        String outputPath = args[1];
        
        SparkConf conf = new SparkConf().setAppName("World Cup Analysis");
        
        JavaSparkContext sc = new JavaSparkContext(conf);
        
//        Build an RDD of Strings from the input textual file.
//        Each string contains the data of a player in the World Cup.
        JavaRDD<String> dataset = sc.textFile(inputPath);
        
//        Remove the header line.
        String header = dataset.first();
        JavaRDD<String> data = dataset.filter(row -> !row.equals(header));
        
//        First Step: Split the players based on the game played and group them.
        JavaPairRDD<String, Iterable<String>> matchesData = data.mapToPair(row -> {
//                    Get the matchId.
                    String matchId = row.split(",")[1];
                    return new Tuple2(matchId, row);
                }
        ).groupByKey();
        
//        Second Step: Determine the winning team for each match and count 
//        the number of players on the field at the end of the game.
        JavaPairRDD<String, Tuple2<String, Integer>> winningTeamsByMatch = matchesData.flatMapValues(playerData -> {
//            Initialize two Map: the first one for the goal scored for each team; 
//            the second one for the number of players on the field at the end of the game for each team; 
            Map<String, Integer> goalsByTeam = new HashMap();
            Map<String, Integer> teamPlayers = new HashMap();

//            For each player we count the number of goal and/or red card.
            for (String player : playerData) {
                String[] fields = player.split(",");
//                Get the three-letters team initial.
                String team = fields[2];
                
//                If the team is not already present in one of the maps (and therefore also in the other), 
//                we insert it in the goals Map with "0" and in the players count Map with "11".
                if(!goalsByTeam.containsKey(team)) {
                    goalsByTeam.put(team, 0);
                    teamPlayers.put(team, 11);
                }
                
//                If the current player doesn't have events we can skip him.
                if(fields.length == 9) {
//                    Take the single events.
                    String[] eventsByPlayer = fields[8].split(" ");

                    for(String events: eventsByPlayer) {
//                        If the event is a Goal or a Penalty we add a goal to the player's team.
                        if(events.startsWith("G") || events.startsWith("P"))
                            goalsByTeam.put(team, goalsByTeam.get(team) + 1);

//                        If the event is an Auto-Goal we subtract a goal to the player's team.
                        if(events.startsWith("W"))
                            goalsByTeam.put(team, goalsByTeam.get(team) - 1);
                        
//                        If the event is a Red Card we subtract a player to the player's team
                        if(events.startsWith("R"))
                            teamPlayers.put(team, teamPlayers.get(team) - 1);
                    }
                }
            }
            
//            Get the two match's team;
            String team1 = (String) goalsByTeam.keySet().toArray()[0];
            String team2 = (String) goalsByTeam.keySet().toArray()[1];
            
//            We use a List of Tuple to get, for each match, the winning team and his number of players.
            List<Tuple2<String, Integer>> winningTeam = new ArrayList();
            
//            We get the winning team based on the goal.
//            In tie case we don't get anythng.
            if(goalsByTeam.get(team1) > goalsByTeam.get(team2))
                winningTeam.add(new Tuple2(team1, teamPlayers.get(team1)));
            else 
                if(goalsByTeam.get(team1) < goalsByTeam.get(team2))
                    winningTeam.add(new Tuple2(team2, teamPlayers.get(team2)));
            
            return winningTeam.iterator();
        });
        
//        Third step: We need to filter the winning teams based on the remaining 
//        number of players, keeping only those with the minimum number of players among all.

//        First of all we get all the number of players and find the min.
        Integer minPlayers = winningTeamsByMatch.map(entry -> entry._2()._2()).reduce((x, y) -> Math.min(x, y));
        
//        After doing this, we need to filter and keep only the matches that were won by 
//        teams with the minimum number of players.
        JavaPairRDD<String, Tuple2<String, Integer>> winningTeamsWithMinPlayers = winningTeamsByMatch.filter(entry -> entry._2()._2().equals(minPlayers));
        
//        Finally, we can extract only the team name (since we kept them separated by match) 
//        and count their occurrences to understand how many matches that team has won with the minimum number of players
        Map<String, Long> winningTeamsWithMinPlayersCountWins = winningTeamsWithMinPlayers.map(entry -> entry._2()._1()).countByValue();
        
//        The count return a map so we have to build a PairRDD with the pair (team, #winsWithMinPlayers).
        List<Tuple2<String, Integer>> temp = new ArrayList();
        for(String team: winningTeamsWithMinPlayersCountWins.keySet()) {
            temp.add(new Tuple2(team, winningTeamsWithMinPlayersCountWins.get(team).intValue()));
        }
        JavaPairRDD<String, Integer> winningTeamsWithMinPlayersCountWinsRDD = sc.parallelizePairs(temp);
        
//        Final step: For the last time, we need to filter the remaining teams to find the one that has won the most.

//        First of all we get all the number of wins and find the max.
        Integer maxWins = winningTeamsWithMinPlayersCountWinsRDD.map(entry -> entry._2()).reduce((x, y) -> Math.max(x, y));
        
//        Finally we filter only the team that has the max number of wins.
//        (We do a final map just to make the printout more readable.)
        JavaRDD<String> teamsWithMaxWinsWithMinPlayers = winningTeamsWithMinPlayersCountWinsRDD.filter(entry -> entry._2().equals(maxWins)).map(entry -> entry._1() + " wins " + maxWins + " match/es with " + minPlayers + " players");
        
        teamsWithMaxWinsWithMinPlayers.saveAsTextFile(outputPath);

        sc.close();
    }
}
