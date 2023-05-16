#include "RecommenderSystem.h"

/***************************
RecommenderSystem member functions
***************************/

sp_movie RecommenderSystem::add_movie(const string& name,int year,const
vector<double>& features)
{
  auto new_movie = std::make_shared<Movie>(name, year);
  _movies[new_movie] = features;
  return new_movie;
}

sp_movie RecommenderSystem::recommend_by_content(const RSUser& user)
{
  double average = calculate_average(user);
  feat_vec pref_vec = create_pref_vec(user, average);
  sp_movie best_movie = find_best_recommendation(user,pref_vec);
  return best_movie;
}


sp_movie RecommenderSystem::recommend_by_cf(const RSUser& user, int k)
{
  auto user_movies = user.get_ranks();
  sp_movie best_movie;
  double cur_highscore = 0;

  //traverse on all existing movies
  for(const auto& movie : _movies)
  {
    //if user didn't watch the movie
    if (!user_movies.count (movie.first))
    {
      double movie_prediction = predict_movie_score (user, movie.first, k);
      if (movie_prediction > cur_highscore)
      {
        cur_highscore = movie_prediction;
        best_movie = movie.first;
      }
    }
  }
  return best_movie;
}


double RecommenderSystem::predict_movie_score(const RSUser &user,
                                              const sp_movie &movie,int k)
{
  auto user_movies = user.get_ranks();

  //create the vector that will contain the highest k similar movies
  vector<pair<double,pair<sp_movie,feat_vec>>> highest_k;
  highest_k.reserve(_movies.size());

  //traverse on all user-watched movies, and for each:
  for(const auto& watched : user_movies)
  {
    //check its similarity with the current non-watched movie
    double similarity = vectors_similarity (_movies.find(watched.first)
                                                ->second, _movies[movie]);
    //insert it to the vector
    highest_k.emplace_back(similarity,make_pair(watched.first,
                                                _movies[watched.first]));
  }

  //throw away the least similar movies, until vector size is k
  while((int) highest_k.size() > k)
  {
    highest_k.erase (min_element(highest_k.begin(), highest_k.end()));
  }
  highest_k.shrink_to_fit();

  //find the score of the movie, based on the k movies in the vector
  double sum =0;
  double sum_mult_rate =0;

  for(auto& watched : highest_k)
  {
    sum += watched.first;
    sum_mult_rate += watched.first* (user_movies[watched.second.first]);
  }
  if(sum == 0)
  {
    throw std::logic_error(ZERO_DIV_ERR_MSG);
  }
  return sum_mult_rate / sum;
}


sp_movie RecommenderSystem::get_movie(const std::string &name, int year) const
{
  for(const auto& movie : _movies)
  {
    if(movie.first->get_name() == name && movie.first->get_year() == year)
    {
      return movie.first;
    }
  }
  return nullptr;
}


ostream& operator<<(std::ostream& output, RecommenderSystem& recsys)
{
  for(auto const& movie : recsys._movies)
  {
    output << *(movie.first);
  }
  return output;
}

/***************************
RecommenderSystem private (helper) functions
***************************/

double RecommenderSystem::vectors_similarity(const vector<double>& a, const
vector<double>& b)
{
  double divided_by = sqrt(vec_scalar_mult(a,a)) * sqrt(vec_scalar_mult(b,b));
  if(divided_by == 0)
  {
    throw logic_error(ZERO_DIV_ERR_MSG);
  }
  return vec_scalar_mult(a,b) / divided_by;
}


double RecommenderSystem::vec_scalar_mult(const vector<double>& a, const
vector<double>& b)
{
  double result = 0;
  for(int i=0; i < (int) a.size(); i++)
  {
    result += a[i]*b[i];
  }
  return result;
}


feat_vec RecommenderSystem::create_pref_vec(const RSUser& user, double avg)
{
  auto user_movies = user.get_ranks();
  feat_vec pref_vec(_movies.begin()->second.size(),DEFAULT_FEAT);
  for(const auto& user_rec_movie : user_movies)
  {
    for(int i = 0;i < (int) pref_vec.size() ;i++)
    {
      pref_vec[i] += ((user_rec_movie.second - avg) *
                      _movies[user_rec_movie.first][i]);
    }
  }
  return pref_vec;
}

sp_movie RecommenderSystem::find_best_recommendation(const RSUser& user,
                                                     const feat_vec& pref_vec)
{
  auto user_movies = user.get_ranks();
  auto best_movie = make_pair (_movies.begin()->first,DEFAULT_FLOAT);
  for(const auto& movie : _movies)
  {
    if(user_movies.count(movie.first) == 0) //if user didn't watch the movie
    {
      double result = vectors_similarity (pref_vec, movie.second);
      if(result > best_movie.second)
      {
        best_movie.first = movie.first;
        best_movie.second = result;
      }
    }
  }
  return best_movie.first;
}


double RecommenderSystem::calculate_average(const RSUser& user)
{
  auto user_movies = user.get_ranks();
  double average = accumulate(user_movies.begin(),user_movies.end
  (),DEFAULT_FLOAT,accumulate_func) /(double) user_movies.size();
  return average;
}


double& RecommenderSystem::accumulate_func(double& sum, const pair<sp_movie,
                                    double>& b)
{
  sum += b.second;
  return sum;
}