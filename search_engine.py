import numpy as np
from matplotlib import pyplot
from itertools import chain

class WebPage:

    def __init__(self, max_info_int, page_length, title_length):
        self.topic = np.rint(np.random.uniform(0, max_info_int))
        self.data = np.rint(np.random.normal(self.topic, 10, page_length))
        self.title = np.random.choice(self.data, title_length, False)


class ActionData:

    def __init__(self, page, info_found):
        self.page = page
        self.info_found = info_found


class SearchEngine:

    def __init__(self, num_pages, max_info_int, page_length, title_length):
        self.ranked_pages = []
        self.unranked_pages = [WebPage(max_info_int, page_length, title_length) for _ in range(num_pages)]
        self.action_data = []

    def rank_pages(self, query):
        query_mean = np.mean(query)
        scores = {page:0 for page in self.ranked_pages}

        for action_query, action_data in self.action_data:
            distance_multiplier = 1 / (abs(query_mean - action_query) + 1)
            scores[action_data.page] += (1 + action_data.info_found) * distance_multiplier

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
        length = min(self.query_length, self.info.size)
        return np.random.choice(self.info, length, False)

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

            if (query.size / (idx + 1)) < max_score and best_page is not None:
                break

            common_info = np.intersect1d(query, page.title)
            page_score = common_info.size / (idx + 1)

            if page_score > max_score or best_page is None:
                best_page = page
                best_idx = idx
                max_score = page_score

        if best_page is not None:
            self.read_pages.add(best_page)

        return best_idx, best_page

    def read_page(self, page):
        _, info_indices, _ = np.intersect1d(self.info, page.data, return_indices=True)
        self.info = np.delete(self.info, info_indices)
        return ActionData(page, info_indices.size)


def iterate(search_engine, max_info_int, user_length, user_std_dev, user_sat_pct, query_length):
    user = User(max_info_int, user_std_dev, user_length, query_length, user_sat_pct)

    num_read = 0
    max_idx = 0

    while not user.is_satisfied():
        query = user.generate_query()
        ranked_pages, unranked_pages = search_engine.rank_pages(query)
        page_idx, page = user.choose_page(query, ranked_pages, unranked_pages)
        if page is None:
            break

        data = user.read_page(page)
        search_engine.record_action(query, data)

        max_idx = max(max_idx, page_idx)
        num_read += 1

    return max_idx, num_read


def run_simulation(num_iterations, num_pages, max_info_int, page_length, page_std_dev,
                   title_length, user_length, user_std_dev, user_sat_pct, query_length):
    print('Creating web pages...')
    search_engine = SearchEngine(num_pages, max_info_int, page_length, title_length)

    max_idxs = []
    nums_read = []
    for i in range(num_iterations):
        print('\rIteration %d' % i, end='')
        max_idx, num_read = iterate(search_engine, max_info_int, user_length,
                                    user_std_dev, user_sat_pct, query_length)
        max_idxs.append(max_idx)
        nums_read.append(num_read)
    print('\nDone.')

    return max_idxs, nums_read


def main():
    max_idxs, nums_read = run_simulation(10000, 1000000, 30000, 100, 10, 20, 50, 10, 0.95, 20)

    with open('output.txt', 'w') as out:
        for out_data in zip(max_idxs, nums_read):
            out.write('%d, %d\n' % out_data)


if __name__ == '__main__':
    main()
