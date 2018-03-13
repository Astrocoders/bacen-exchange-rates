open Jest;

test("to convert properly from json", () => {
  open Daily;
  let currencyJsonStr = {|
  {"@odata.context":"https://was-p/olinda/servico/PTAX/versao/v1/odata$metadata#_CotacaoMoedaDia","value":[{"paridadeCompra":1.2295,"paridadeVenda":1.2299,"cotacaoCompra":4.0072,"cotacaoVenda":4.0092,"dataHoraCotacao":"2018-03-05 10:02:47.732","tipoBoletim":"Abertura"},{"paridadeCompra":1.2306,"paridadeVenda":1.2310,"cotacaoCompra":4.0098,"cotacaoVenda":4.0118,"dataHoraCotacao":"2018-03-05 11:05:54.755","tipoBoletim":"Intermediário"},{"paridadeCompra":1.2323,"paridadeVenda":1.2324,"cotacaoCompra":4.0153,"cotacaoVenda":4.0164,"dataHoraCotacao":"2018-03-05 12:03:31.119","tipoBoletim":"Intermediário"},{"paridadeCompra":1.2327,"paridadeVenda":1.2330,"cotacaoCompra":4.0117,"cotacaoVenda":4.0134,"dataHoraCotacao":"2018-03-05 13:05:07.46","tipoBoletim":"Intermediário"},{"paridadeCompra":1.2327,"paridadeVenda":1.2330,"cotacaoCompra":4.0156,"cotacaoVenda":4.0174,"dataHoraCotacao":"2018-03-05 13:05:07.476","tipoBoletim":"Fechamento PTAX"}]}
  |};
  Expect.(
    expect(currencyJsonStr |> Json.parseOrRaise |> ExchangeRateDecode.decode)
    |> toEqual(
         ExchangeRateDecode.(
           [|
             {
               buyParity: 1.2295,
               sellParity: 1.2299,
               buyPrice: 4.0072,
               sellPrice: 4.0092,
               date: "2018-03-05 10:02:47.732" |> Js.Date.fromString,
               report: Opening,
             },
             {
               buyParity: 1.2306,
               sellParity: 1.2310,
               buyPrice: 4.0098,
               sellPrice: 4.0118,
               date: "2018-03-05 11:05:54.755" |> Js.Date.fromString,
               report: Intermediary,
             },
             {
               buyParity: 1.2323,
               sellParity: 1.2324,
               buyPrice: 4.0153,
               sellPrice: 4.0164,
               date:
                 "2018-03-05 12:03:31.119"
                 |> Js.Date.fromString,
               report: Intermediary,
             },
             {
               buyParity: 1.2327,
               sellParity: 1.2330,
               buyPrice: 4.0117,
               sellPrice: 4.0134,
               date:
                 "2018-03-05 13:05:07.46"
                 |> Js.Date.fromString,
               report: Intermediary,
             },
             {
               buyParity: 1.2327,
               sellParity: 1.2330,
               buyPrice: 4.0156,
               sellPrice: 4.0174,
               date:
                 "2018-03-05 13:05:07.476"
                 |> Js.Date.fromString,
               report: PTAXClosing,
             },
           |]
         ),
       )
  );
});

test("to not raise with empty", () => {
  open Daily;
  let currencyJsonStr = {|
  {"@odata.context":"https://was-p/olinda/servico/PTAX/versao/v1/odata$metadata#_CotacaoMoedaDia","value":[]}
  |};
  Expect.(
    expect(currencyJsonStr |> Json.parseOrRaise |> ExchangeRateDecode.decode)
    |> toEqual(
         (
           [||]
         ),
       )
  );
});
