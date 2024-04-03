> [!IMPORTANT]
> DText was moved to the [Danbooru](https://github.com/danbooru/danbooru) repository in commit [d965f959b](https://github.com/danbooru/danbooru/commit/d965f959b49b15fe048a625d32b2db2807127286). See [lib/dtext_rb](https://github.com/danbooru/danbooru/tree/master/lib/dtext_rb#readme) in the Danbooru repo.

---

# DText

A Ruby library for parsing [DText](https://danbooru.donmai.us/wiki_pages/help:dtext),
[Danbooru](https://github.com/danbooru/danbooru)'s text formatting language.

# Installation

```bash
sudo apt-get install ruby ruby-dev g++ libc-dev make patch xz-utils ragel
gem install bundler
bundle install
bin/rake
```

# Usage

```bash
ruby -Ilib -rdtext -e 'puts DText.parse("hello world")'
# => <p>hello world</p>
```

# Development

```bash
bin/rake clean
bin/rake compile
bin/rake test
bin/rake bench
```

To build in debug mode:

```bash
bin/rake clean
DTEXT_DEBUG=true bin/rake compile
```

To build with Clang:

```bash
MAKE="make --environment-overrides" CXX="/usr/bin/clang++" bin/rake
```
