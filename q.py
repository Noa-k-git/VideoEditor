from typing import List
from dataclasses import dataclass
from abc import ABC

@dataclass
class Item(ABC):
    
    def get_str_keys(self, between: str, before:str = ""):
        s = between.join([before + key for key in self.keys])
        # s = s[:-1*len(between)]
        return s
    def __iter__(self):
        for key, _ in self.__annotations__.items():
            value =getattr(self, key)
            if value != None:
                yield key, str(value)
    # def keys(self):
    #     return True
        # return self
    # def __next__(self):
    #     self.idx = -1
    #     return self.__dict__.items()[self.idx]
    # def __iter__(self):
    #     # return {k: str(v) for k, v in self.__dict__.items() if v is not None}
    #     for key, value in self.__dict__.items():
    #         if value != None:
    #             yield key, str(value)
    # def __dict__(self):
    #     # return {k: str(v) for k, v in self}
    #     return {k:str(v) for k, v in self.__dict__}
    # def __getattr__(self, name):
    #     if name == 'keys' and isinstance(self, Item):
    #         return [k for k, _ in self]
    #     if name == 'values' and isinstance(self,Item):
    #         return [v for _, v in self]
    #     else:
    #         raise AttributeError(f"'{self.__class__.__name__}' object has no attribute '{name}'")
    @property
    def properties(self) -> List[str]:
        return list(dict(self).keys())
    @property
    def data(self) -> List[str]:
        return list(dict(self).values())
        # return [str(v) for _, v in self.__dict__.items() if v != None]

    # def __repr__(self):
    #     return str(dict(self))
    

@dataclass
class User(Item):
    id: int = None
    name: str = "kj"
    email: str = None
    password: bytes = None

print(dict(User()).keys())