#  Ciaravola Giosuè, 0622702177, g.ciaravola3@studenti.unisa.it
#  Attended course: AH
#  Prof. D'Aniello Giuseppe, gidaniello@unisa.it
#  
#  Exercise 2 – Spark
#  Find the team (or teams in case of a tie) that has won the most matches 
#  with the fewest number of players on the field at the end of the game.
#
/opt/bitnami/spark/bin/spark-submit \
  --class worldcup.SparkDriver \
  --master local \
  ./WorldCupAnalysisSpark.jar \
  ./input ./outputSingle
