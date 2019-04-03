import numpy as np
from matplotlib import pyplot
from itertools import chain

class WebPage:

    def __init__(self, max_info_int, page_length):
        self.topic = np.rint(np.random.uniform(0, max_info_int))
        self.data = np.rint(np.random.normal(self.topic, 10, page_length))


class ActionData:

    def __init__(self, page, info_found):
        self.page = page
        self.info_found = info_found


class SearchEngine:

    def __init__(self, num_pages, max_info_int, page_length):
        self.ranked_pages = []
        self.unranked_pages = [WebPage(max_info_int, page_length) for _ in range(num_pages)]
        self.action_data = {}

    def rank_pages(self, query):
        scores = {page:0 for page in self.ranked_pages}

        for action_query, action_data in self.action_data.items():
            for data in action_data:
                distance_multiplier = 1 / (abs(query - action_query) + 1)
                scores[data.page] += (1 + data.info_found) * distance_multiplier

        self.ranked_pages.sort(key=lambda page: scores[page], reverse=True)

        return self.ranked_pages, self.unranked_pages

    def record_action(self, query, data):
        if query in self.action_data:
            self.action_data[query].append(data)
        else:
            self.action_data[query] = [data]
        
        if data.page not in self.ranked_pages:
            self.ranked_pages.append(data.page)
            self.unranked_pages.remove(data.page)


class User:

    def __init__(self, max_info_int, info_length, satisfy_pct):
        topic = np.rint(np.random.uniform(0, max_info_int))
        self.info = np.rint(np.random.normal(topic, 10, info_length))
        self.info_length = info_length
        self.satisfy_pct = satisfy_pct
        self.read_pages = set()

    def generate_query(self):
        return np.mean(self.info)

    def is_satisfied(self):
        return (self.info.size / self.info_length) < (1 - self.satisfy_pct)

    def choose_page(self, query, ranked_pages, unranked_pages):
        all_pages = chain(ranked_pages, unranked_pages)

        best_page = None
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
                max_score = page_score

        if best_page is not None:
            self.read_pages.add(best_page)

        return best_page

    def read_page(self, page):
        _, info_indices, _ = np.intersect1d(self.info, page.data, return_indices=True)
        self.info = np.delete(self.info, info_indices)
        return ActionData(page, info_indices.size)


def iterate(search_engine):
    user = User(50000, 50, 0.95)

    num_read = 0

    query = None
    ranked_pages = None
    unranked_pages = None

    while not user.is_satisfied():
        new_query = user.generate_query()
        if new_query != query:
            query = new_query
            ranked_pages, unranked_pages = search_engine.rank_pages(query)

        page = user.choose_page(query, ranked_pages, unranked_pages)
        if page is None:
            break

        data = user.read_page(page)
        search_engine.record_action(query, data)
        num_read += 1

    return num_read


def main():
    print('Creating web pages...')
    search_engine = SearchEngine(1000000, 50000, 100)

    nums_read = []
    for i in range(300):
        print('\rIteration %d' % i, end='')
        num_read = iterate(search_engine)
        nums_read.append(num_read)
    print('\nDone.')

    with open('output.txt', 'w') as out:
        for num in nums_read:
            out.write(str(num) + '\n')


if __name__ == '__main__':
    main()
