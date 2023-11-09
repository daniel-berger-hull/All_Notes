module.exports = {
    selectAllQuestionSets: "SELECT * from declaration.question_set",
    createNewQuestionSet: function(questionSet) {
        // The code is not important here, just the WAY THE FUNCTION IS EXPORTED!
        return "INSERT INTO declaration.+ " + uestionSet.appId + ";
         
    },

    createQuestionSetParameters: function(setId) {
      // The code is not important here, just the WAY THE FUNCTION IS EXPORTED!
        return "INSERT INTO ..." +
            setId + "', '"  "') RETURNING id;";
    }
}