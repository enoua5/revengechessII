function indexOfElement(el)
{
  let i = 0;
  while( (el = el.previousSibling) != null)
    i++;
  return i;
}
