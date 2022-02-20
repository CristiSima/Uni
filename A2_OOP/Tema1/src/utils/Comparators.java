package utils;

import fileio.ActorInputData;
import fileio.ActionInputData;
import systems.Ratings;
import systems.User;
import systems.Video;
import common.Constants;

import java.util.Comparator;
import java.util.function.Predicate;
import java.util.regex.Pattern;

/**
 * Comparators and elimination filters
 */
public final class Comparators {
    /**
     * gets the comparator order modifier
     */
    public static int getOrder(final String order) {
        if (order.equals("asc")) {
            return 1;
        } else {
            return -1;
        }
    }

    private Comparators() { }
    /**
     * comparators for videos
     */
    public static class ForVideo {
        public static class FavoriteComparator implements Comparator<Video> {
            private int order;
            public FavoriteComparator(final String order) {
                this.order = Comparators.getOrder(order);
            }
            /**
             * sort by the number of favorite designations
             */
            @Override
            public int compare(final Video video1, final Video video2) {
                int res =  (video1.getFavorites()
                 - video2.getFavorites())
                 * this.order;
                if (res == 0) {
                    return video1.getTitle().compareTo(video2.getTitle())
                           * this.order;
                }
                return res;
            }
            /**
             * filters out videos without any favorite designation
             */
            public static boolean filter(final Video video) {
                return video.getFavorites() != 0;
            }
        }
        public static class ViewComparator implements Comparator<Video> {
            private int order;
            public ViewComparator(final String order) {
                this.order = Comparators.getOrder(order);
            }
            /**
             * sort by the total number of views
             */
            @Override
            public int compare(final Video video1, final Video video2) {
                int res = (video1.getViews()
                 - video2.getViews())
                 * this.order;
                if (res == 0) {
                    return video1.getTitle().compareTo(video2.getTitle())
                           * this.order;
                }
                return res;
            }
            /**
             * filter out videos without views
             */
            public static boolean filter(final Video video) {
                return video.getViews() != 0;
            }
        }
        public static class RatingsComparator implements Comparator<Video> {
            private int order;
            public RatingsComparator(final String order) {
                this.order = Comparators.getOrder(order);
            }
            /**
             * sort by ratings
             */
            @Override
            public int compare(final Video video1, final Video video2) {
                int res = Double.compare(video1.getRatings(),
                  video2.getRatings())
                 * this.order;
                if (res == 0) {
                    return video1.getTitle().compareTo(video2.getTitle())
                           * this.order;
                }
                return res;
            }
            /**
             * filters out unrated videos
             */
            public static boolean filter(final Video video) {
                return video.getRatings() != 0;
            }
        }
        public static class RatingsComparatorString implements Comparator<String> {
            private int order;
            public RatingsComparatorString(final String order) {
                this.order = Comparators.getOrder(order);
            }
            /**
             * sort by ratings
             */
            @Override
            public int compare(final String title1, final String title2) {
                Ratings ratings = Ratings.getRatings();
                int res = Double.compare(ratings.getForVideo(title1),
                  ratings.getForVideo(title2))
                 * this.order;
                if (res == 0) {
                    return title1.compareTo(title2)
                           * this.order;
                }
                return res;
            }
            /**
             * filters out unrated videos
             */
            public static boolean filter(final String title) {
                return Ratings.getRatings().getForVideo(title) != 0;
            }
        }
        public static class DurationComparator implements Comparator<Video> {
            private int order;
            public DurationComparator(final String order) {
                this.order = Comparators.getOrder(order);
            }
            /**
             * sort by the duration of a video
             */
            @Override
            public int compare(final Video video1, final Video video2) {
                int res = (video1.getDuration()
                 - video2.getDuration())
                 * this.order;
                if (res == 0) {
                    return video1.getTitle().compareTo(video2.getTitle())
                           * this.order;
                }
                return res;
            }
            /**
             * filters out videos that don't exist?
             */
            public static boolean filter(final Video video) {
                return video.getDuration() != 0;
            }
        }
    }

    public static class ForUser {
        public static class NumRatingsComparator implements Comparator<User> {
            private int order;
            public NumRatingsComparator(final String order) {
                this.order = Comparators.getOrder(order);
            }
            /**
             * sort by the number of videos rated
             */
            @Override
            public int compare(final User user1, final User user2) {
                int res = Double.compare(user1.getRatings(),
                  user2.getRatings())
                 * this.order;
                if (res == 0) {
                    return user1.getUsername().compareTo(user2.getUsername())
                           * this.order;
                }
                return res;
            }
            /**
             * filters out users the haven't rated any video
             */
            public static boolean filter(final User user) {
                return user.getRatings() != 0;
            }
        }
    }

    public static class ForActor {
        public static class AverageRatingsComparator implements Comparator<ActorInputData> {
            private int order;
            public AverageRatingsComparator(final ActionInputData parameters) {
                this.order = Comparators.getOrder(parameters.getSortType());
            }
            /**
             * sort by the actor's (indirect) rating
             */
            @Override
            public int compare(final ActorInputData actor1, final ActorInputData actor2) {
                Ratings ratings = Ratings.getRatings();

                int res = Double.compare(ratings.getForActor(actor1),
                  ratings.getForActor(actor2))
                 * this.order;

                if (res == 0) {
                    return actor1.getName().compareTo(actor2.getName())
                           * this.order;
                }
                return res;
            }
            /**
             * filters out actors the haven't been rated(indirectly)
             */
            public static boolean filter(final ActorInputData actor) {
                return Ratings.getRatings().getForActor(actor) != 0;
            }
        }

        public static class AwardsComparator implements Comparator<ActorInputData> {
            private int order;
            public AwardsComparator(final ActionInputData parameters) {
                this.order = Comparators.getOrder(parameters.getSortType());
            }
            /**
             * sorts by the number of awards
             */
            @Override
            public int compare(final ActorInputData actor1, final ActorInputData actor2) {
                int res = 0;

                for (int awards:actor1.getAwards().values()) {
                    res += awards;
                }

                for (int awards:actor2.getAwards().values()) {
                    res -= awards;
                }
                res *= this.order;
                if (res == 0) {
                    return actor1.getName().compareTo(actor2.getName())
                           * this.order;
                }
                return res;
            }
            /**
             * filters out actors that dont have specified awards
             */
            public static Predicate<ActorInputData> getfilter(final ActionInputData parameters) {
                return (actor) -> {
                           for (String award:parameters.getFilters()
                                                        .get(Constants.INPUT_AWARDS_OFFSET)) {
                               if (!actor.getAwards().containsKey(Utils.stringToAwards(award))) {
                                   return false;
                               }
                           }
                           return true;
                };
            }
        }

        public static class DescriptionComparator implements Comparator<ActorInputData> {
            private int order;
            public DescriptionComparator(final ActionInputData parameters) {
                this.order = Comparators.getOrder(parameters.getSortType());
            }
            /**
             * sort by name
             */
            @Override
            public int compare(final ActorInputData actor1, final ActorInputData actor2) {
                int res = actor1.getName().compareTo(actor2.getName())
                 * this.order;
                if (res == 0) {
                    return actor1.getName().compareTo(actor2.getName())
                           * this.order;
                }
                return res;
            }
            /**
             * filters out actors that don't have specified keywords in their description
             */
            public static Predicate<ActorInputData> getfilter(final ActionInputData parameters) {
                return (actor) -> {
                           for (final String word:parameters.getFilters()
                                                            .get(Constants.INPUT_KEYWORD_OFFSET)) {
                               Pattern pattern = Pattern.compile("\\b" + word + "\\b");
                               if (!pattern.matcher(actor.getCareerDescription().toLowerCase())
                                                    .find()) {
                                   return false;
                               }
                           }
                           return true;
                };
            }
        }
    }
}
