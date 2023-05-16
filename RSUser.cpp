
#include "RSUser.h"
#include "RecommenderSystem.h"


/***************************
   RSUser member functions
***************************/

void RSUser::add_movie_to_rs(const std::string &name, int year,
                     const std::vector<double> &features,double rate)
{
  auto added_movie = _rec_sys->add_movie (name, year, features);
  _rec_movies[added_movie] = rate;
}


sp_movie RSUser::get_recommendation_by_content() const
{
  return _rec_sys->recommend_by_content (*this);
}


sp_movie RSUser::get_recommendation_by_cf(int k) const
{
  return _rec_sys->recommend_by_cf (*this, k);
}


double RSUser::get_prediction_score_for_movie(const std::string& name, int
year, int k) const
{
  auto movie_to_predict = _rec_sys->get_movie (name, year);
  return _rec_sys->predict_movie_score(*this,movie_to_predict,k);
}


/**************Operators**************/
ostream& operator<<(ostream& o, const RSUser& user)
{
  o<<NAME_PRINT << user.get_name() <<'\n' << *(user._rec_sys) << endl;
  return o;
}
