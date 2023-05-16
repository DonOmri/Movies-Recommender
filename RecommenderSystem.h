#ifndef SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
#define SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
#include "RSUser.h"

#include <cmath>
#include <map>
#include <numeric>
#include <algorithm>

#define DEFAULT_FEAT 0
#define DEFAULT_FLOAT 0.0
#define ZERO_DIV_ERR_MSG "Division by zero is not allowed"

using std::vector;
using std::pair;
using std::accumulate;
using std::logic_error;

typedef vector<double> feat_vec;


class RecommenderSystem
{
  std::map<sp_movie,std::vector<double>,equal_func> _movies;

  /**
   * sums the multiplication of each of the first vector coordinates
   * with the correspondant coordinate of the second vector
   * @param a first vector
   * @param b second vector
   * @return the sum
   */
  static double vec_scalar_mult(const vector<double>& a,const
  vector<double>& b);

  /**
   * tests similarity between 2 given vectors, using given formula in pdf
   * @param a first vector
   * @param b second vector
   * @return the similraity of the vector, a double between 0 and 1
   */
  static double vectors_similarity(const vector<double>& a,const
  vector<double>& b);

  /**
   * creates a preference vector for a user
   * @param user the user to create the preference vector to
   * @param avg the user average rating to their seen movies
   * @return the preference vector
   */
  feat_vec create_pref_vec(const RSUser& user, double avg);

  /**
   * finds the highest rated non-seen movie for a user, using the
   * preferences vector
   * @param user the user to recommend to
   * @param pref_vec the user's preference vector
   * @return the movie with the highest prediction
   */
  sp_movie find_best_recommendation(const RSUser& user, const feat_vec&
  pref_vec);

  /**
   * calculates the average rating for user seen movies
   * @param user the user to calculate the average to
   * @return the average rating
   */
  static double calculate_average(const RSUser& user);

  /**
   * helper func to use std::accumulate on the user rank_map
   * @param sum the current sum
   * @param b the pair that is pointed by the map iterator
   * @return the sum of both
   */
  static double& accumulate_func(double& sum, const pair<sp_movie,
      double>& b);

public:

	explicit RecommenderSystem(): _movies(map_equal_func) {}
    /**
     * adds a new movie to the system
     * @param name name of movie
     * @param year year it was made
     * @param features features for movie
     * @return shared pointer for movie in system
     */
	sp_movie add_movie(const std::string& name,int year,const
    std::vector<double>& features);


    /**
     * a function that calculates the movie with highest score based on
     * movie features
     * @param ranks user ranking to use for algorithm
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_content(const RSUser& user);

    /**
     * a function that calculates the movie with highest predicted score
     * based on ranking of other movies
     * @param ranks user ranking to use for algorithm
     * @param k
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_cf(const RSUser& user, int k);


    /**
     * Predict a user rating for a movie given argument using item cf
     * procedure with k most similar movies.
     * @param user_rankings: ranking to use
     * @param movie: movie to predict
     * @param k:
     * @return score based on algorithm as described in pdf
     */
	double predict_movie_score(const RSUser &user, const sp_movie &movie,
												  int k);

	/**
	 * gets a shared pointer to movie in system
	 * @param name name of movie
	 * @param year year movie was made
	 * @return shared pointer to movie in system
	 */
	sp_movie get_movie(const std::string &name, int year) const;


  friend ostream& operator<<(std::ostream& output, RecommenderSystem& recsys);
  friend bool Movie::operator<(const Movie& rhs);
};


#endif //SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H