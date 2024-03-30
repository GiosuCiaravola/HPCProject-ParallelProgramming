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

import java.io.*;
import java.util.*;
import org.apache.hadoop.io.Writable;

/*
*   Utility class to keep the match and all the goals of that match for a player.
*/

public class PlayerMatchGoal implements Writable {
    private String match;
    private List<String> goals;
    
    public PlayerMatchGoal() {
        this.match = "";
        this.goals = new ArrayList<>();
    } 

    public PlayerMatchGoal(String match) {
        this.match = match;
        this.goals = new ArrayList<>();
    }

    public String getMatch() {
        return match;
    }

    public List<String> getGoals() {
        return goals;
    }
    
    public void addGoal(String goal) {
        goals.add(goal);
    }
    
    @Override
    public void write(DataOutput out) throws IOException {
        out.writeUTF(match);
        out.writeInt(goals.size());
        for (String goal : goals) {
            out.writeUTF(goal);
        }
    }

    @Override
    public void readFields(DataInput in) throws IOException {
        match = in.readUTF();
        int size = in.readInt();
        goals = new ArrayList<>(size);
        for (int i = 0; i < size; i++) {
            goals.add(in.readUTF());
        }
    }
    
//    Override the toString in order to structure it.
    @Override
    public String toString() {
        if(goals.isEmpty())
            return "";
        else {
            String matchGoal = match + ": ";
            for(String goal: goals) {
                matchGoal = matchGoal.concat(goal + ",");
            }
            return matchGoal.substring(0, matchGoal.length() - 1);
        }
    }
}
