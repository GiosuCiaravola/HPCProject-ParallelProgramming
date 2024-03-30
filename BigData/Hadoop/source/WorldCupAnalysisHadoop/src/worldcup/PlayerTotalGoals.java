/*  
*   Ciaravola Giosuè, 0622702177, g.ciaravola3@studenti.unisa.it
*   Attended course: AH
*   Prof. D'Aniello Giuseppe, gidaniello@unisa.it
*   
*   Exercise 1 - Map Reduce
*
*   Develop a program using Hadoop Map Reduce to analyze the available dataset differently from the second exercise
*   (e.g., a statistic, a ranking, an index, etc.). 
*
*   For this task, the decision has been made to calculate the ranking of players who have scored the most goals overall. 
*   The number of players considered for the ranking is taken from the command line. 
*   Additionally, for each player, all the goals scored are listed, as an inverted index, indicating in which match they were made.
*/

package worldcup;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import org.apache.hadoop.io.WritableComparable;

/*
*   Utility class to keep the playerName his goalNumber and the index of theese goals.
*/

public class PlayerTotalGoals implements WritableComparable<PlayerTotalGoals> {
    private String playerName;
    private Integer goalsNumber;
    private String goals;

    public PlayerTotalGoals() {
    }

    public PlayerTotalGoals(String playerName, Integer goalsNumber, String goals) {
        this.playerName = playerName;
        this.goalsNumber = goalsNumber;
        this.goals = goals;
    }

    public String getPlayerName() {
        return playerName;
    }

    public Integer getGoalsNumber() {
        return goalsNumber;
    }

    public String getGoals() {
        return goals;
    }
    
    @Override
    public void write(DataOutput out) throws IOException {
        out.writeUTF(playerName);
        out.writeInt(goalsNumber);
        out.writeUTF(goals);
    }

    @Override
    public void readFields(DataInput in) throws IOException {
        playerName = in.readUTF();
        goalsNumber = in.readInt();
        goals = in.readUTF();
    }
    
    @Override
    public int compareTo(PlayerTotalGoals other) {
//        We compare two PlayerTotalGoals based on their goals number.
        int result = other.goalsNumber.compareTo(this.getGoalsNumber());
        
//        In tie case we use the player Name alphabetical order.
        if (result == 0) {
            result = this.playerName.compareTo(other.getPlayerName());
        }

        return result;
    }
    
    @Override
    public String toString() {
        return playerName + "\t" + goals;
    }
}
