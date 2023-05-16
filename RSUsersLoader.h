#ifndef SCHOOL_SOLUTION_USERFACTORY_H
#define SCHOOL_SOLUTION_USERFACTORY_H

#include "RSUser.h"

#define END_OF_USER_DATA '\n'
#define REPLACE_THIS '-'
#define WITH_THIS ' '
#define CUT_TO_GET_USER_NAME ' '
#define CUT_TO_GET_MOVIE_RATE ' '
#define ILLEGAL_RTG_MSG "One of the user ratings is illegal"
#define NA_TEST_IDX 0
#define IS_NEGATIVE '-'

using std::istream;
using std::istringstream;
using std::ifstream;
using std::vector;
using std::unique_ptr;
using std::runtime_error;
using std::string;


class RSUsersLoader
{
private:
  /**
   * creates a vector of movies from a given string of movies
   * @param movies_str string of movies
   * @param rs the recommender system the users are using
   * @return vector of all the movies from the users file
   */
  static vector<sp_movie> create_movies_vector(string& movies_str, const
  RecommenderSystem& rs);

  /**
   * creates a user (including his movie ratings) from a given stream of data
   * @param rs the recommender system the user is using
   * @param details user's name and movie ratings, as a string seperated by
   * " "
   * @param movies_vec vector with the movies of the recommendation system
   * @return
   */
  static RSUser create_user(shared_ptr<RecommenderSystem>& rs, istream&
  details, const vector<sp_movie>& movies_vec);

public:
    RSUsersLoader() = delete;
    /**
     *
     * loads users by the given format with their movie's ranks
     * @param users_file_path a path to the file of the users and their
     * movie ranks
     * @param rs RecommendingSystem for the Users
     * @return vector of the users created according to the file
     */
    static std::vector<RSUser> create_users_from_file(const std::string&
    users_file_path, unique_ptr<RecommenderSystem> rs) noexcept(false);
};

#endif //SCHOOL_SOLUTION_USERFACTORY_H