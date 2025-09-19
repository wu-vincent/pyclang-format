from pyclang_format import format

hello_world = """#include<iostream>
int main(){std::cout<<    "Hello World"<<std::endl;




return 0;
}"""

hello_world_format = """#include <iostream>
int main() {
  std::cout << "Hello World" << std::endl;

  return 0;
}"""


def test_format():
    assert format(hello_world, style="LLVM") == hello_world_format
