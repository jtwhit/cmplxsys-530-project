import numpy as np
from matplotlib import pyplot
from itertools import chain

class WebPage:

    def __init__(self, max_info_int, page_length):
        self.topic = np.rint(np.random.uniform(0, max_info_int))
        self.data = np.rint(np.random.normal(self.topic, 10, page_length))


class ActionData:

    def __init__(self, page, topic_similarity, info_found):
        self.page = page
        self.topic_similarity = topic_similarity
        self.info_found = info_found


class SearchEngine:

    def __init__(self, num_pages, max_info_int, page_length, weights):
        self.ranked_pages = []
        self.unranked_pages = [WebPage(max_info_int, page_length) for _ in range(num_pages)]
        self.action_data = []
        self.max_info_int = max_info_int
        self.page_length = page_length
        self.weights = weights


    def rank_pages(self, query):
        scores = {page:0 for page in self.ranked_pages}

        for action_query, action_data in self.action_data:
            score = (self.weights[0] +
                     self.weights[1] * (1 - (action_data.topic_similarity * 2) / self.max_info_int) +
                     self.weights[2] * (action_data.info_found / self.page_length))
            distance_multiplier = 1 / (abs(query - action_query) + 1)
            scores[action_data.page] += score * distance_multiplier

        self.ranked_pages.sort(key=lambda page: scores[page], reverse=True)

        return self.ranked_pages, self.unranked_pages

    def record_action(self, query, data):
        query_mean = np.mean(query)
        self.action_data.append((query_mean, data))
        
        if data.page not in self.ranked_pages:
            self.ranked_pages.append(data.page)
            self.unranked_pages.remove(data.page)


class User:

    def __init__(self, max_info_int, info_std_dev, info_length, query_length, satisfy_pct):
        topic = np.rint(np.random.uniform(0, max_info_int))
        self.info = np.rint(np.random.normal(topic, info_std_dev, info_length))
        self.info_length = info_length
        self.query_length = query_length
        self.satisfy_pct = satisfy_pct
        self.read_pages = set()

    def generate_query(self):
        return np.mean(self.info)

    def is_satisfied(self):
        return (self.info.size / self.info_length) < (1 - self.satisfy_pct)

    def choose_page(self, query, ranked_pages, unranked_pages):
        all_pages = chain(ranked_pages, unranked_pages)

        best_page = None
        best_idx = 0
        max_score = 0
        for idx, page in enumerate(all_pages):
            if page in self.read_pages:
                continue

            if -(idx + 1) < max_score and best_page is not None:
                break

            topic_score = -abs(query - page.topic)
            page_score = (topic_score - 1) * (idx + 1)

            if page_score > max_score or best_page is None:
                best_page = page
                best_idx = idx
                max_score = page_score

        if best_page is not None:
            self.read_pages.add(best_page)

        return best_idx, best_page

    def read_page(self, query, page):
        topic_similarity = abs(query - page.topic)
        _, info_indices, _ = np.intersect1d(self.info, page.data, return_indices=True)
        self.info = np.delete(self.info, info_indices)
        return ActionData(page, topic_similarity, info_indices.size)


def iterate(search_engine, max_info_int, user_length, user_std_dev, user_sat_pct, query_length):
    user = User(max_info_int, user_std_dev, user_length, query_length, user_sat_pct)

    max_idx = 0
    num_read = 0

    query = None
    ranked_pages = None
    unranked_pages = None

    while not user.is_satisfied():
        new_query = user.generate_query()
        if new_query != query:
            query = new_query
            ranked_pages, unranked_pages = search_engine.rank_pages(query)

        page_idx, page = user.choose_page(query, ranked_pages, unranked_pages)
        if page is None:
            break

        data = user.read_page(query, page)
        search_engine.record_action(query, data)

        max_idx = max(max_idx, page_idx)
        num_read += 1

    return max_idx, num_read


def run_simulation(num_iterations, num_pages, max_info_int, page_length, page_std_dev,
                   weights, user_length, user_std_dev, user_sat_pct, query_length):
    print('Creating web pages...')
    search_engine = SearchEngine(num_pages, max_info_int, page_length, weights)

    max_idxs = []
    num_reads = []
    for i in range(num_iterations):
        print('\rIteration %d' % i, end='')
        max_idx, num_read = iterate(search_engine, max_info_int, user_length,
                                    user_std_dev, user_sat_pct, query_length)
        max_idxs.append(max_idx)
        num_reads.append(num_read)
    print('\nDone.')

    return max_idxs, num_reads


def sweep_parameter(name, param_idx, params):
    default_params = (5000, 100000, 10000, 100, 10, (1/3, 1/3, 1/3), 50, 10, 0.95, 20)
    num_runs = 10
    last_n = 100

    for i in range(num_runs):
        max_idx_means = []
        num_read_means = []

        for param in params:
            run_params = default_params[:param_idx] + (param,) + default_params[param_idx+1:]
            max_idxs, num_reads = run_simulation(*run_params)
            max_idx_means.append(np.mean(max_idxs[-last_n:]))
            num_read_means.append(np.mean(num_reads[-last_n:]))

        with open('%s_%d.txt' % (name, i), 'w') as out:
            for out_data in zip(max_idx_means, num_read_means):
                out.write('%f, %f\n' % out_data)


def sweep_num_pages():
    nums = [1000, 5000, 10000, 20000, 50000, 100000, 500000]
    sweep_parameter('num_pages', 1, nums)


def sweep_max_info_int():
    max_infos = [100, 500, 1000, 5000, 10000, 20000, 50000]
    sweep_parameter('max_infos', 2, max_infos)


def sweep_weights():
    weights = [(1, 0 ,0),
               (0, 1, 0),
               (0, 0, 1),
               (1/2, 1/2, 0),
               (1/2, 0, 1/2),
               (0, 1/2, 1/2),
               (1/3, 1/3, 1/3)]

    sweep_parameter('weights', 5, weights)


def main():
    sweep_weights()


if __name__ == '__main__':
    main()
