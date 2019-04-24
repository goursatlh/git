import scrapy

class QuotesSpider(scrapy.Spider):
    name = "quotes"
    start_urls = [ "http://quotes.toscrape.com" ]

    def parse(self, response):
        for quote in  response.css('div.quote'):
            yield {
                    'text': quote.css('span.text::text').get(),
                    'author': quote.css('small.author::text').get()
            }

        next_page = response.css('li.next a::attr(href)').get()
        print(next_page)
        if next_page is not None:
            next_page = response.urljoin(next_page)
            print(next_page)
            yield scrapy.Request(next_page, callback=self.parse)

